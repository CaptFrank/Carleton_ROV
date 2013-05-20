/**********************************************************************
*
*    Author:		shondll (shondll@yahoo.com)
*    Company:		-
*    Date:			14/06/2010
*    File Version:	1.00
*    Description:	main driver file
*
**********************************************************************/

#ifndef _XBEE_DRIVER_H
#define _XBEE_DRIVER_H

// command codes for device
typedef enum _XBEE_CMD {
	XBEE_CMD_NONE = 0x00, // not part of commands, for programmer's purposes
	XBEE_CMD_STATUS = 0x8A, // < from the modem
	XBEE_CMD_ATCMD = 0x08, // > to the modem
	XBEE_CMD_ATCMD_QUEUE = 0x09, // > to the modem
	XBEE_CMD_ATCMD_RESPONSE = 0x88, // < from the modem
	XBEE_CMD_TX_16 = 0x01, // > to the modem
	XBEE_CMD_TX_64 = 0x00, // > to the modem
	XBEE_CMD_TX_STATUS = 0x89, // < from the modem
	XBEE_CMD_RX_16 = 0x81, // < from the modem
	XBEE_CMD_RX_64 = 0x80 // < from the modem
} XBEE_CMD;

// user issued commands
typedef enum _XBEE_USER_CMD {
	XBEE_USRCMD_NONE = 0x00,
	XBEE_USRCMD_HELP,
	XBEE_USRCMD_STATUS,
	XBEE_USRCMD_ATCMD,
	XBEE_USRCMD_SEND_16,
	XBEE_USRCMD_SEND_64,
	XBEE_USRCMD_RCV_16,
	XBEE_USRCMD_RCV_64
} XBEE_USER_CMD;

// structure of the status of XBEE module, as seen by the user
typedef struct _XBEE_STATUS {
	int hwReset :1;
	int wdtReset :1;
	int associated :1;
	int disassociated :1;
	int syncLost :1;
	int coordRealignment :1;
	int coordStarted :1;

	int txStatus;
	int rxStatus;
} XBEE_STATUS;

// these are for the above described structure
#define XBEE_TX_STATUS_IDLE 0
#define XBEE_TX_STATUS_PROGRESS 1
#define XBEE_TX_STATUS_ERROR -1
#define XBEE_RX_STATUS_IDLE 0
#define XBEE_RX_STATUS_16 1
#define XBEE_RX_STATUS_64 2



#endif // _XBEE_DRIVER_H


