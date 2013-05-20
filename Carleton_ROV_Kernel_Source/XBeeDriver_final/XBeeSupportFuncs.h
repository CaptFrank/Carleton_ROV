/**********************************************************************
*
*    Author:		shondll (shondll@yahoo.com)
*    Company:		-
*    Date:			13/06/2010
*    File Version:	1.00
*    Description:	Support functions for the XBee API mode
*
**********************************************************************/

#ifndef _XBEE_SUPPORT_FUNCS_H
#define _XBEE_SUPPORT_FUNCS_H

#include "XBeeDriver.h"

// special symbols
#define FRAME_DELIMITER		((BYTE)0x7E)
#define ESCAPE_CHARACTER	((BYTE)0x7D)
#define XON_CHARACTER		((BYTE)0x11)
#define XOFF_CHARACTER		((BYTE)0x13)

BYTE XBeeApiCalcChecksum(BYTE *pData, int len);
BOOL XBeeApiCheckChecksum(BYTE *pData, int len);

int XBeeApiEscapeData(BYTE *pInData, int len, BYTE *pOutData);
int XBeeApiUnescapeData(BYTE *pInData, int len, BYTE *pOutData);

int XBeeApiAssembleCommand(XBEE_CMD cmd, BYTE *pData, int dataLen, BYTE *pOutData);
XBEE_CMD XBeeApiUnassembleCommand(BYTE *pData, int dataLen, BYTE *pOutData, int *pOutDataLen);


#endif // _XBEE_SUPPORT_FUNCS_H

