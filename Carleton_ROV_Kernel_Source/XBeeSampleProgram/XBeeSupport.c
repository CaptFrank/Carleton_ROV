/**********************************************************************
*
*    Author:		shondll (shondll_yahoo.com)
*    Company:		-
*    Date:			27/11/2010
*    File Version:	1.00
*    Description:	Support functions applications using XBee devices
*
**********************************************************************/
#include "XBeeSupport.h"
#include <string.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>

/* DEFINE LOCAL TYPES HERE */

/* DEFINE LOCAL CONSTANTS HERE */

/* DEFINE LOCAL VARIABLES HERE */

/* DECLARE LOCAL FUNCTIONS HERE */

/* DECLARE LOCAL FUNCTIONS HERE */
static int WriteDeviceFile(char *filename, unsigned char *pData, int datalen); // use to write commands into device

static int ReadDeviceFile(char *filename, unsigned char *pData, int *datalen); // use to retrieve responces from device

static int OpenDeviceFile(char *filename); // used by read/write operations
static void CloseDeviceFile(int handle); // used by read/write operations

static int HexStr2ByteArr(char *pInData, char *pOutData);
static int ByteArr2HexStr(char *pInData, int inDataLen, char *pOutData);
/* DEFINE FUNCTIONS HERE */

/******************************************************************************
* Description: XBeeSend16(..) - send a message to a 16bit address desination
* Input: 	filename - the name of the device file to use
*			destAddr - destination address, 0x0000 - 0xFFFF
*			msg - the payload of the message
*			msgLen - length of the message payload
* Output: 	none
* Return:	0 - successfully sent
*			1 - not acknowledged (may be sent however)
*			-2 - error during sending
*			-1 - some other error
*******************************************************************************/
int XBeeSend16(char *filename, unsigned int destAddr, unsigned char *msg, int msglen)
{
	char buf[256];
	int responseSize;
	
	// append command and address
	if(destAddr > 0xFFFF)
		return -1;
	sprintf(buf, "SEND16 %04X ", destAddr);
	
	// apend message
	if(msglen > 100)
		return -1;
	ByteArr2HexStr(msg, msglen, buf + strlen(buf));
	
	if(WriteDeviceFile(filename, buf, strlen(buf)) != 0)
		return -1;
		
	// wait some time for responce to be generated
	sleep(1);
		
	// read status of device
	responseSize = sizeof(buf);
	if(ReadDeviceFile(filename, buf, &responseSize) != 0)
		return -1;
		
	if(strstr(buf, "TX_STATUS SUCCESS") != NULL)
		return 0;
	else if(strstr(buf, "TX_STATUS NACK") != NULL)
		return 1;
	else
		return -2;
}

/******************************************************************************
* Description: XBeeRcv16(..) - receive a message if avalable from a 16bit source address
* Input: 	filename - the name of the device file to use
* Output: 	srcAddr - source address, 0x0000 - 0xFFFF
*			msg - the payload of the message (must provide at least 100 bytes)
*			msgLen - length of the message payload
* Return:	0 - no msg available
*			1 - msg successfully read
*			-1 - some error
*******************************************************************************/
int XBeeRcv16(char *filename, unsigned int *srcAddr, unsigned char *msg, int *msglen)
{
	char buf[256];
	char msgBuf[256];
	int responseSize;
	int rssi;
	int flags;
	
	// check status of device
	strcpy(buf, "STATUS");
	if(WriteDeviceFile(filename, buf, strlen(buf)) != 0)
		return -1;
		
	// read status of device
	responseSize = sizeof(buf);
	if(ReadDeviceFile(filename, buf, &responseSize) != 0)
		return -1;
		
	//  is there a msg?
	if(strstr(buf, "RX_STATUS 16_BIT_MSG") == NULL)
		return 0; // NO
		
	// get msg
	strcpy(buf, "RCV16");
	if(WriteDeviceFile(filename, buf, strlen(buf)) != 0)
		return -1;
	responseSize = sizeof(buf);
	if(ReadDeviceFile(filename, buf, &responseSize) != 0)
		return -1;
	buf[responseSize] = '\0';
		
	// translate msg
	if(sscanf(buf, "ADDRESS 0x%X\nRSSI %ddBm\nFLAGS 0x%X\nMSG %s", srcAddr, &rssi, &flags, msgBuf) != 4) 			return -1;
		
	*msglen = HexStr2ByteArr(msgBuf, msg);
	
	return 1;
}

/******************************************************************************
* Description: XBeeSetShortAddr(..) - change the short address of the device
* Input: 	filename - the name of the device file to use
*			addr - address of the device, 0x0000 - 0xFFFF
* Output: 	none
* Return:	0 - successfully set new address
*			-1 - error occured in operation
*******************************************************************************/
int XBeeSetShortAddr(char *filename, unsigned int addr)
{
	char buf[64];
	int responseSize;
	
	// append command and address
	if( (addr > 0xFFFF) || (addr == 0) )
		return -1;
	sprintf(buf, "AT MY %04X", addr);	
	if(WriteDeviceFile(filename, buf, strlen(buf)) != 0)
		return -1;
		
	// wait some time for responce to be generated
	sleep(1);
		
	// read status of device
	responseSize = sizeof(buf);
	if(ReadDeviceFile(filename, buf, &responseSize) != 0)
		return -1;
		
	if(strstr(buf, "MY, OK") != NULL)
		return 0;
		
	return -1;
}

/******************************************************************************
* Description: XBeeGetShortAddr(..) - read the current short address of the device
* Input: 	filename - the name of the device file to use
* Output: 	addr - address of the device, 0x0000 - 0xFFFF
* Return:	0 - operation successfully executed
*		-1 - error occured in operation
*******************************************************************************/
int XBeeGetShortAddr(char *filename, unsigned int *addr)
{
	char buf[64];
	int responseSize;
	
	// append command and address
	sprintf(buf, "AT MY");
	if(WriteDeviceFile(filename, buf, strlen(buf)) != 0)
		return -1;
		
	// wait some time for responce to be generated
	sleep(1);
		
	// read status of device
	responseSize = sizeof(buf);
	if(ReadDeviceFile(filename, buf, &responseSize) != 0)
		return -1;
		
	if(sscanf(buf, "MY, OK, %X", addr) == 1)
		return 0;
		
	return -1;
}


/* DEFINE FUNCTIONS HERE */
static int WriteDeviceFile(char *filename, unsigned char *pData, int datalen)
{

//	printf("\n%s", pData);

	int result = 0;

	int handle = OpenDeviceFile(filename);
	if(handle == -1)
		return -1;
		
	if(write(handle, pData, datalen) != datalen)
		result = -1;
		
	CloseDeviceFile(handle);
	
	return result;

}

static int ReadDeviceFile(char *filename, unsigned char *pData, int *datalen)
{
	int result = 0;
	int dataLenTmp = *datalen;

	int handle = OpenDeviceFile(filename);
	if(handle == -1)
		return -1;
		
	if((dataLenTmp = read(handle, pData, dataLenTmp)) < 0)
		result = -1;
	else
		*datalen = dataLenTmp;
		
	CloseDeviceFile(handle);
	
	return result;	
}

static int OpenDeviceFile(char *filename)
{
	int handle;

	handle = open(filename, O_RDWR | O_NOCTTY);

	return handle;
}

static void CloseDeviceFile(int handle)
{
	close(handle);
}

// this is a helper
// converts hex input string to byte array
static int HexStr2ByteArr(char *pInData, char *pOutData)
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










