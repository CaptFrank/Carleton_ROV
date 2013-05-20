/**********************************************************************
*
*    Author:		shondll (shondll_yahoo.com)
*    Company:		-
*    Date:		14/06/2010
*    File Version:	1.00
*    Description:	main driver file
*
**********************************************************************/
#include "includes.h"

MODULE_LICENSE("Dual BSD/GPL");

/* DEFINE LOCAL TYPES HERE */
typedef struct _BUFFER_CS {
	char *p; // pointer to the buffer
	int size; // size of the buffer
	int i; // current index
} BUFFER_CS; // CS means like in C#

/* DEFINE LOCAL CONSTANTS HERE */
#define XBEE_DEVICE_NAME "XBee0"
#define XBEE_MAJOR_NUMBER 61

/* DEFINE LOCAL VARIABLES HERE */
static BUFFER_CS Response; // all responses from the modem should be printed in thid var (dynamically allocated)
static XBEE_STATUS XBeeStatusFlags; // current status of modem is held here
static BUFFER_CS RawInputBuffer;
static BUFFER_CS AtCommandResponseBuffer;
static BUFFER_CS Recv16DataBuffer;
static BUFFER_CS Recv64DataBuffer;

static XBEE_USER_CMD lastCommandIssued;


/* DECLARE LOCAL FUNCTIONS HERE */
int XBeeInit(void);
void XBeeExit(void);
int XBeeOpen(struct inode *inode, struct file *filp);
int XBeeClose(struct inode *inode, struct file *filp);
ssize_t XBeeRead(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t XBeeWrite(struct file *filp, const char *buf, size_t count, loff_t *f_pos);

int HexStr2ByteArr(char *pInData, char *pOutData);
int ByteArr2HexStr(char *pInData, int inDataLen, char *pOutData);
static void XBeePrintStatus(void);
static void XBeePrintHelp(void);
static void RecognseInputCommand(void);
static void ProcessInputDataStream(void);

/* DRIVER STUFF HERE */
/* Structure that declares the usual file */
/* access functions */
struct file_operations xbee_fops = {
  .read = XBeeRead,
  .write = XBeeWrite,
  .open = XBeeOpen,
  .release = XBeeClose
};

/* Declaration of the init and exit functions */
module_init(XBeeInit);
module_exit(XBeeExit);

/* DEFINE FUNCTIONS HERE */
int XBeeInit(void)
{
	int result = 0;

	/* Registering device */
	result = register_chrdev(XBEE_MAJOR_NUMBER, XBEE_DEVICE_NAME, &xbee_fops);
	if (result < 0) {
		printk("XBee: Cannot obtain major number %d\n", XBEE_MAJOR_NUMBER);
		return result;
	}

	result = SpOpen();
	if(result) {
		printf("XBee: Unable to open serial port file!\n");
		goto fail;
	} else {
		printf("XBee: serial port file opened!\n");
	}

	lastCommandIssued = XBEE_CMD_NONE;
	XBeeStatusFlags.hwReset = 0;
	XBeeStatusFlags.wdtReset = 0;
	XBeeStatusFlags.associated = 0;
	XBeeStatusFlags.disassociated = 0;
	XBeeStatusFlags.syncLost = 0;
	XBeeStatusFlags.coordRealignment = 0;
	XBeeStatusFlags.coordStarted = 0;
	XBeeStatusFlags.txStatus = -1;
	XBeeStatusFlags.rxStatus = -1;

	/* Additional initialization */
	CommandParserInit();
	
	printf("\nXBee: Module inserted successfully!\n");	
	return 0;

fail:
	XBeeExit();
	return result;
}

void XBeeExit(void) 
{
	/* Freeing the major number */
	unregister_chrdev(XBEE_MAJOR_NUMBER, XBEE_DEVICE_NAME);

	// close serial port
	if( SpOpened() )
		SpClose();

	if(Response.p)
		free(Response.p);
		
	printf("\nXBee: Module removed!\n");
}

int XBeeOpen(struct inode *inode, struct file *filp) 
{
	int result = 0;

	Response.i = 0;
	Response.p = NULL;
	Response.size = 0;

	return result;
}

int XBeeClose(struct inode *inode, struct file *filp) 
{ 
	int result = 0;

	if(Response.p) {
		free(Response.p);
		Response.p = NULL;
	}

	return result;
}

ssize_t XBeeRead(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{ 
	int readCount = 0;

	// check for any useful data in input buffer
	ProcessInputDataStream();

	// sanity checks
	if(count < 1)
		return 0;

	if(Response.p == NULL || Response.i == 0) {
		// initialize response structure
		if(Response.p) {
			free(Response.p);
			Response.p = NULL;
		}
		Response.i = 0;
		Response.size = 0;

		switch(lastCommandIssued) {
			case XBEE_USRCMD_NONE:
				break;

			case XBEE_USRCMD_HELP:
				XBeePrintHelp();
				break;

			case XBEE_USRCMD_STATUS:
				XBeePrintStatus();
				break;
			case XBEE_USRCMD_SEND_16:
			case XBEE_USRCMD_SEND_64:
				XBeePrintStatus();
				XBeeStatusFlags.txStatus = -1;
				break;

			case XBEE_USRCMD_ATCMD:
				Response.p = AtCommandResponseBuffer.p;
				Response.size = AtCommandResponseBuffer.size;
				AtCommandResponseBuffer.p = NULL;
				break;

			case XBEE_USRCMD_RCV_16:
				Response.p = Recv16DataBuffer.p;
				Response.size = Recv16DataBuffer.size;
				Recv16DataBuffer.p = NULL;
				if(Recv64DataBuffer.p)
					XBeeStatusFlags.rxStatus = 2;
				else
					XBeeStatusFlags.rxStatus = 0;
				break;

			case XBEE_USRCMD_RCV_64:
				Response.p = Recv64DataBuffer.p;
				Response.size = Recv64DataBuffer.size;
				Recv64DataBuffer.p = NULL;
				if(Recv16DataBuffer.p)
					XBeeStatusFlags.rxStatus = 1;
				else
					XBeeStatusFlags.rxStatus = 0;
				break;
		}

		// disable further printing of the same command
		lastCommandIssued = XBEE_USRCMD_NONE;
	}

	// nothing more to read (should not get here!)
	if( (Response.p == NULL) || (Response.i >= Response.size) ) {
		readCount = 0;

		// set ready flag for possible next command
		Response.i = 0;
	}
	// data available less than data requested - send available
	else if(Response.i + (int)count >= Response.size) {
		int tmp = Response.size - Response.i;

		copy_to_user(buf, Response.p + Response.i, tmp);
		Response.i += tmp;
		readCount = tmp;

		// set ready flag for possible next command
		Response.i = 0;
	}
	// more data is available than requested
	else {
		copy_to_user(buf, Response.p + Response.i, count);
		Response.i += count;
		readCount = count;
	}

	*f_pos += readCount; 
	return readCount;
}

ssize_t XBeeWrite( struct file *filp, const char *buf, size_t count, loff_t *f_pos) 
{
	BYTE *p, *pDataToSend;
	int dataToSendLen;
	char *str;

	// some very crude size constraint
	if(count > 200)
		return 0;

	str = (char*)malloc(count + 1);
	if(str == NULL)
		return 0;

	// check for any useful data in input buffer
	ProcessInputDataStream();

	// copy over to kernel space
	copy_from_user(str, buf, count);
	str[count] = '\0';

	lastCommandIssued = XBEE_USRCMD_NONE;

	// parse input command
	while(str) {
		p = NULL;
		pDataToSend = NULL;
		dataToSendLen = 0;

		str = CommandParserParse(str);

		switch( CommandParserGetCurrentCommand() ) {
		default:
		case XBEE_USRCMD_NONE:
			break;

		case XBEE_USRCMD_HELP:
			lastCommandIssued = XBEE_USRCMD_HELP;
			printf("XBee: issued XBEE_USRCMD_HELP.\n");
			break;

		case XBEE_USRCMD_STATUS:
			// information should be automatically received from modem, nothing to send!
			lastCommandIssued = XBEE_USRCMD_STATUS;
			printf("XBee: issued XBEE_USRCMD_STATUS.\n");
			break;

		case XBEE_USRCMD_ATCMD:
			// at least the AT command code should be present
			if( CommandParserGetParamsNum() < 1 )
				break;

			p = (BYTE*)malloc(128);
			pDataToSend = (BYTE*)malloc(256);
			if( !p || !pDataToSend) 
				break;
			
			p[0] = 0x55; // append frame ID, dummy!
			if(strlen(CommandParserGetParam(0)) != 2) // AT command code should be exactly 2 characters
				break;
			strcpy( p + 1, CommandParserGetParam(0) ); //append AT command code
			dataToSendLen = 1 + strlen(p + 1);
			if( CommandParserGetParamsNum() > 1 ) { // add parameter if there is one
				// convert to byte array
				dataToSendLen += HexStr2ByteArr( CommandParserGetParam(1), p + dataToSendLen);
			}

			dataToSendLen = XBeeApiAssembleCommand(XBEE_CMD_ATCMD, p, dataToSendLen, pDataToSend);
			lastCommandIssued = XBEE_USRCMD_ATCMD;
			printf("XBee: issued XBEE_USRCMD_ATCMD.\n");
			break;

		case XBEE_USRCMD_SEND_16:
			// address and data should be supplied as parameters
			if( CommandParserGetParamsNum() < 2 )
				break;

			p = (BYTE*)malloc(128);
			pDataToSend = (BYTE*)malloc(256);
			if( !p || !pDataToSend) 
				break;
			
			p[0] = 0x56; // append frame ID, dummy!
			dataToSendLen = 1;

			// append address
			if(strlen(CommandParserGetParam(0)) != 4) // address should be exactly 4 characters long
				break;
			dataToSendLen += HexStr2ByteArr( CommandParserGetParam(0), p + dataToSendLen);

			// append options
			p[dataToSendLen++] = 0x00; // no options for the moment

			// append the message
			dataToSendLen += HexStr2ByteArr( CommandParserGetParam(1), p + dataToSendLen);

			dataToSendLen = XBeeApiAssembleCommand(XBEE_CMD_TX_16, p, dataToSendLen, pDataToSend);

			lastCommandIssued = XBEE_USRCMD_SEND_16;
			printf("XBee: issued XBEE_USRCMD_SEND_16.\n");
			break;

		case XBEE_USRCMD_SEND_64:
			// address and data should be supplied as parameters
			if( CommandParserGetParamsNum() < 2 )
				break;

			p = (BYTE*)malloc(128);
			pDataToSend = (BYTE*)malloc(256);
			if( !p || !pDataToSend) 
				break;
			
			p[0] = 0x57; // append frame ID, dummy!
			dataToSendLen = 1;

			// append address
			if(strlen(CommandParserGetParam(0)) != 16) // address should be exactly 16 characters long
				break;
			dataToSendLen += HexStr2ByteArr( CommandParserGetParam(0), p + dataToSendLen);

			// append the message
			dataToSendLen += HexStr2ByteArr( CommandParserGetParam(1), p + dataToSendLen);

			dataToSendLen = XBeeApiAssembleCommand(XBEE_CMD_TX_64, p, dataToSendLen, pDataToSend);
			lastCommandIssued = XBEE_USRCMD_SEND_64;
			printf("XBee: issued XBEE_USRCMD_SEND_64.\n");
			break;

		case XBEE_USRCMD_RCV_16:
			if(XBeeStatusFlags.rxStatus == 1) {
				lastCommandIssued = XBEE_USRCMD_RCV_16;
			}
			printf("XBee: issued XBEE_USRCMD_RCV_16.\n");
			break;

		case XBEE_USRCMD_RCV_64:
			if(XBeeStatusFlags.rxStatus == 2) {
				lastCommandIssued = XBEE_USRCMD_RCV_64;
			}
			printf("XBee: issued XBEE_USRCMD_RCV_64.\n");
			break;
		}

		// if there is smth to send, send it
		if(pDataToSend && dataToSendLen && lastCommandIssued != XBEE_USRCMD_NONE)
			SpWrite(pDataToSend, dataToSendLen);
			
		// free resources
		if(pDataToSend)	free(pDataToSend);
		if(p) free(p);

		CommandParserDiscardCurrentCommand();
	}

	free(str);

	// check for any useful data in input buffer
	ProcessInputDataStream();

	*f_pos += count;
	return count;
}

static void XBeePrintStatus(void)
{
	char *txStat, *rxStat;

	switch(XBeeStatusFlags.txStatus) {
	case 0: txStat = "SUCCESS"; break;
	case 1: txStat = "NACK"; break;
	case 2: txStat = "CCA FAIL"; break;
	case 3: txStat = "PURGED"; break;
	default: txStat = "UNKNOWN"; break;
	}

	if(XBeeStatusFlags.rxStatus == 1)
		rxStat = "16_BIT_MSG";
	else if(XBeeStatusFlags.rxStatus == 2)
		rxStat = "64_BIT_MSG";
	else 
		rxStat = "IDLE";

	if(Response.p) free(Response.p); // free previously occupied space
	Response.p = (char*)malloc(1024);
	if(Response.p) {
		sprintf(Response.p,
			"STATUS \n"
			"HW_RESET %c\n"
			"WDT_RESET %c\n"
			"ASSOCIATED %c\n"
			"DISASSOCIATED %c\n"
			"SYNC_LOST %c\n"
			"COORD_REALIGNMENT %c\n"
			"COORD_STARTED %c\n"
			"TX_STATUS %s\n"
			"RX_STATUS %s\n",
			XBeeStatusFlags.hwReset ? 'y' : 'n',
			XBeeStatusFlags.wdtReset ? 'y' : 'n',
			XBeeStatusFlags.associated ? 'y' : 'n',
			XBeeStatusFlags.disassociated ? 'y' : 'n',
			XBeeStatusFlags.syncLost ? 'y' : 'n',
			XBeeStatusFlags.coordRealignment ? 'y' : 'n',
			XBeeStatusFlags.coordStarted ? 'y' : 'n',
			txStat,
			rxStat
		);
			
		Response.i = 0;
		Response.size = strlen(Response.p);
	}
}

static void XBeePrintHelp(void)
{
	if(Response.p) free(Response.p); // free previously occupied spac
	Response.p = (char*)malloc(1024);
	if(Response.p) {
		strcpy(Response.p,
			"Supported commands:\n"
			"\tSTATUS (print current device status)\n"
			"\tHELP (prints this help)\n"
			"\tAT XX [YY...Y] (sends AT command to device, XX is the command code, YY...Y is parameter in HEX)\n"
			"\tSEND16 XXXX YY...Y (send msg to another node using 16bit address, XXXX is the address in HEX, YY...Y is the msg in HEX)\n"
			"\tSEND64 XXXXXXXXXXXXXXXX YY...Y (send msg to another node using 64bit address, XXXXXXXXXXXXXXXX is the address in HEX, YY...Y is the msg in HEX)\n"
			"\tRCV16 (receive msg received and stored in local buffer from another node identified by 16bit adress)\n"
			"\tRCV64 (receive msg received and stored in local buffer from another node identified by 64bit adress)\n"
		);

		Response.i = 0;
		Response.size = strlen(Response.p);
	}
}

static void ProcessInputDataStream(void)
{
#define	DATA_CHUNK_SIZE 20
	BUFFER_CS localBuf;
	char b[DATA_CHUNK_SIZE];
	BYTE *pTmp;
	
	localBuf.p = b;

	localBuf.size = 0;
//	printf("XBee: Start of read...\n");
//	while( (localBuf.size = SpRead(localBuf.p, DATA_CHUNK_SIZE)) > 0) {
//		printf("XBee: Read %d from serial port.\n", localBuf.size);
//	}
//	printf("XBee: End of read (%d)...\n", localBuf.size);

	if(!RawInputBuffer.p) {
		if( (RawInputBuffer.p = (char*)malloc(DATA_CHUNK_SIZE)) == NULL )
			return;
		RawInputBuffer.size = DATA_CHUNK_SIZE;
		RawInputBuffer.i = 0;
	}

	// loop through all available data in serial port buffer
	while( (localBuf.size = SpRead(localBuf.p, DATA_CHUNK_SIZE)) > 0) {
		printf("XBee: Read %d from serial port.\n", localBuf.size);
		// look for the start of a new command
		for(localBuf.i = 0; localBuf.i < localBuf.size; localBuf.i++) {
			if(localBuf.p[localBuf.i] == FRAME_DELIMITER) {
				// force recognition of possible command in buffer
				RecognseInputCommand();
				free(RawInputBuffer.p);
				RawInputBuffer.p = NULL;

				// start adding new bytes
				if( (RawInputBuffer.p = (char*)malloc(DATA_CHUNK_SIZE)) == NULL )
					return;
				RawInputBuffer.size = DATA_CHUNK_SIZE;
				RawInputBuffer.p[0] = FRAME_DELIMITER;
				RawInputBuffer.i = 1;
			} else {
				// add byte to raw input buffer
				if(RawInputBuffer.i >= RawInputBuffer.size) {
					if( (pTmp = malloc(RawInputBuffer.size + DATA_CHUNK_SIZE)) == NULL )
						return;
					memcpy(pTmp, RawInputBuffer.p, RawInputBuffer.size);
					free(RawInputBuffer.p);
					RawInputBuffer.p = pTmp;					
					RawInputBuffer.size += DATA_CHUNK_SIZE;
				}
				RawInputBuffer.p[RawInputBuffer.i++] = localBuf.p[localBuf.i];
			}
		}

	}
	printf("XBee: Read %d from serial port!\n", localBuf.size);

	// try to recognise a command
	printf("XBee: Recognising command.\n");
	RecognseInputCommand();

}

static void RecognseInputCommand(void)
{
	XBEE_CMD cmd;
	BUFFER_CS localBuf;

	localBuf.p = (char*)malloc(RawInputBuffer.i);

	cmd = XBeeApiUnassembleCommand(RawInputBuffer.p, RawInputBuffer.i, localBuf.p, &localBuf.size);

	if(cmd != XBEE_CMD_NONE)
		printf("XBee: Recognised 0x%X command.\n", cmd);
	else {
		int i;
		
		printf("XBee: Didn't recognise anything!\n");
		printf("XBee: BYTES: ");		
		for(i = 0; i < RawInputBuffer.i; i++)
			printf("%X,", RawInputBuffer.p[i]);
		printf("\n");
	}

	switch(cmd) {
	case XBEE_CMD_NONE:
		break;
	case XBEE_CMD_STATUS:
		if(localBuf.size == 1)
		{
			// byte 0 contains status information
			BYTE tmp = localBuf.p[0];
			XBeeStatusFlags.hwReset = (tmp & 0x01) ? 1 : 0;
			XBeeStatusFlags.wdtReset = (tmp & 0x02) ? 1 : 0;
			XBeeStatusFlags.associated = (tmp & 0x04) ? 1 : 0;
			XBeeStatusFlags.disassociated = (tmp & 0x08) ? 1 : 0;
			XBeeStatusFlags.syncLost = (tmp & 0x10) ? 1 : 0;
			XBeeStatusFlags.coordRealignment = (tmp & 0x20) ? 1 : 0;
			XBeeStatusFlags.coordStarted = (tmp & 0x40) ? 1 : 0;
		}
		free(RawInputBuffer.p); RawInputBuffer.p = NULL;
		break;

	case XBEE_CMD_ATCMD_RESPONSE:
		if(AtCommandResponseBuffer.p) {
			free(AtCommandResponseBuffer.p);
		}
		AtCommandResponseBuffer.i = 0;
		if( (AtCommandResponseBuffer.p = (char*)malloc(256)) ) {
			char *str;
			// byte 3 contains status of execution of last command
			switch(localBuf.p[3]) {
			case 0: str = "OK"; break;
			case 1: str = "ERROR"; break;
			case 2: str = "INV CMD"; break;
			case 3: str = "INV PAR"; break;
			default: str = "UNKNOWN"; break;
			}
			// print command code, status and any message if available
			sprintf(AtCommandResponseBuffer.p, "%c%c, %s, ", localBuf.p[1], localBuf.p[2], str);
			AtCommandResponseBuffer.i = strlen(AtCommandResponseBuffer.p);
			AtCommandResponseBuffer.size = AtCommandResponseBuffer.i + 
				ByteArr2HexStr(localBuf.p + 4, localBuf.size - 4, AtCommandResponseBuffer.p + AtCommandResponseBuffer.i);
			AtCommandResponseBuffer.p[AtCommandResponseBuffer.size++] = '\n';
			AtCommandResponseBuffer.p[AtCommandResponseBuffer.size] = '\0';
		}
		free(RawInputBuffer.p); RawInputBuffer.p = NULL;
		break;

	case XBEE_CMD_TX_STATUS:
		if(localBuf.size == 2) {
			BYTE tmp = localBuf.p[1];
			XBeeStatusFlags.txStatus = tmp & 0x03;
		}
		free(RawInputBuffer.p); RawInputBuffer.p = NULL;
		break;
	case XBEE_CMD_RX_16:
		if(Recv16DataBuffer.p) {
			free(Recv16DataBuffer.p);
		}
		Recv16DataBuffer.i = 0;
		if( (Recv16DataBuffer.p = (char*)malloc(256)) ) {
			char addressStr[10], flagsStr[5];

			ByteArr2HexStr(localBuf.p, 2, addressStr);
			ByteArr2HexStr(localBuf.p + 3, 1, flagsStr);

			sprintf(Recv16DataBuffer.p, "ADDRESS 0x%s\nRSSI -%ddBm\nFLAGS 0x%s\nMSG ", addressStr, localBuf.p[2], flagsStr);
			// now copy the received message if any
			Recv16DataBuffer.i = strlen(Recv16DataBuffer.p);
			Recv16DataBuffer.size = Recv16DataBuffer.i + 
				ByteArr2HexStr(localBuf.p + 4, localBuf.size - 4, Recv16DataBuffer.p + Recv16DataBuffer.i);	
			Recv16DataBuffer.p[Recv16DataBuffer.size++] = '\n';
			Recv16DataBuffer.p[Recv16DataBuffer.size] = '\0';

			XBeeStatusFlags.rxStatus = 1;
		}
		free(RawInputBuffer.p); RawInputBuffer.p = NULL;
		break;
	case XBEE_CMD_RX_64:
		if(Recv64DataBuffer.p) {
			free(Recv64DataBuffer.p);
		}
		Recv64DataBuffer.i = 0;
		if( (Recv64DataBuffer.p = (char*)malloc(256)) ) {
			char addressStr[20], flagsStr[5];

			ByteArr2HexStr(localBuf.p, 8, addressStr);
			ByteArr2HexStr(localBuf.p + 9, 1, flagsStr);

			sprintf(Recv64DataBuffer.p, "ADDRESS 0x%s\nRSSI -%ddBm\nFLAGS 0x%s\nMSG ", addressStr, localBuf.p[8], flagsStr);
			// now copy the received message if any
			Recv64DataBuffer.i = strlen(Recv64DataBuffer.p);
			Recv64DataBuffer.size = Recv64DataBuffer.i + 
				ByteArr2HexStr(localBuf.p + 8, localBuf.size - 8, Recv64DataBuffer.p + Recv64DataBuffer.i);	
			Recv64DataBuffer.p[Recv64DataBuffer.size++] = '\n';
			Recv64DataBuffer.p[Recv64DataBuffer.size] = '\0';

			XBeeStatusFlags.rxStatus = 2;
		}
		free(RawInputBuffer.p); RawInputBuffer.p = NULL;
		break;
		
	default:
		break;
	}

	free(localBuf.p);
}


// this is a helper
// converts hex input string to byte array
int HexStr2ByteArr(char *pInData, char *pOutData)
{
	int inStrLen, outStrLen, tmp;
	BOOL hiNibble;
	
	// to upper (seems to not be supported)
	//strtoupr(pInData);
	
	inStrLen = strlen(pInData);
	outStrLen = (inStrLen / 2) + (inStrLen % 2);	
	
	hiNibble = FALSE;
	tmp = outStrLen - 1;
	while(inStrLen--) {
		char ch = pInData[inStrLen];
				
		if( ((ch >= '0') && (ch <= '9')) || ((ch >= 'A') && (ch <= 'F')) ) {
			if((ch >= '0') && (ch <= '9'))	
				ch -= '0';
			if((ch >= 'A') && (ch <= 'F'))	
				ch = ch - 'A' + 0xA;
			
			if(hiNibble)
				pOutData[tmp--] |= ch << 4;
			else
				pOutData[tmp] = ch;				
			
			hiNibble = !hiNibble;
		}
		else {
			return 0; // met character outside range
		}	
	}
	
	return outStrLen;
}

// this is a helper
// converts input byte array to hex string 
int ByteArr2HexStr(char *pInData, int inDataLen, char *pOutData)
{
	int i;

	for(i = 0; i < inDataLen; i++) {
		char tmp;

		tmp = (pInData[i] >> 4) & 0xF;
		if(tmp > 9)	tmp += 'A' - 0xA; else tmp += '0';
		*pOutData++ = tmp;

		tmp = (pInData[i] >> 0) & 0xF;
		if(tmp > 9)	tmp += 'A' - 0xA; else tmp += '0';
		*pOutData++ = tmp;
	}

	*pOutData = '\0';

	return i * 2;
}
