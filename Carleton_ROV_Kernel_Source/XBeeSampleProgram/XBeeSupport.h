/**********************************************************************
*
*    Author:		shondll (shondll_yahoo.com)
*    Company:		-
*    Date:			27/11/2010
*    File Version:	1.00
*    Description:	Support functions for applications using XBee devices
*
**********************************************************************/

#ifndef XBEE_SUPPORT_H
#define XBEE_SUPPORT_H

// YES, THERE IS NO INITIALIZING FUNCTION!

#ifndef BOOL
typedef enum _BOOL { FALSE = 0, TRUE = 1 } BOOL;
#endif


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
int XBeeSend16(char *filename, unsigned int destAddr, unsigned char *msg, int msglen);

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
int XBeeRcv16(char *filename, unsigned int *srcAddr, unsigned char *msg, int *msglen);

/******************************************************************************
* Description: XBeeSetShortAddr(..) - change the short address of the device
* Input: 	filename - the name of the device file to use
*			addr - address of the device, 0x0000 - 0xFFFF
* Output: 	none
* Return:	0 - successfully set new address
*			-1 - error occured in operation
*******************************************************************************/
int XBeeSetShortAddr(char *filename, unsigned int addr);

/******************************************************************************
* Description: XBeeGetShortAddr(..) - read the current short address of the device
* Input: 	filename - the name of the device file to use
* Output: 	addr - address of the device, 0x0000 - 0xFFFF
* Return:	0 - operation successfully executed
*		-1 - error occured in operation
*******************************************************************************/
int XBeeGetShortAddr(char *filename, unsigned int *addr);

#endif // XBEE_SUPPORT_H









