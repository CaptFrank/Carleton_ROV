/**********************************************************************
*
*    Author:		shondll (shondll@yahoo.com)
*    Company:		-
*    Date:			13/06/2010
*    File Version:	1.00
*    Description:	Support functions for the XBee API mode
*
**********************************************************************/
#include "includes.h"

/* DEFINE LOCAL TYPES HERE */

/* DEFINE LOCAL CONSTANTS HERE */

/* DEFINE LOCAL VARIABLES HERE */

/* DECLARE LOCAL FUNCTIONS HERE */

/* DECLARE LOCAL FUNCTIONS HERE */

/* DEFINE FUNCTIONS HERE */

BYTE XBeeApiCalcChecksum(BYTE *pData, int len)
{
	int i, sum = 0;

	for(i = 0; i < len; i++) {
		sum += pData[i];
	}

	return ( (BYTE)0xFF - (BYTE)sum );
}

BOOL XBeeApiCheckChecksum(BYTE *pData, int len)
{
	int i, sum = 0;

	for(i = 0; i < len; i++) {
		sum += pData[i];
	}

	return ( (sum & 0xFF) == 0xFF);
}

int XBeeApiEscapeData(BYTE *pInData, int len, BYTE *pOutData)
{
	int i, j;

	for(i = 0, j = 0; i < len; i++) {
		BYTE tmp = pInData[i];

		if( (tmp == FRAME_DELIMITER) ||
			(tmp == ESCAPE_CHARACTER) ||	
			(tmp == XON_CHARACTER) ||
			(tmp == XOFF_CHARACTER) )
		{ // character has to be escaped
			pOutData[j++] = ESCAPE_CHARACTER;
			pOutData[j++] = pInData[i] ^ (BYTE)0x20;
		} 
		else {  // normal character
			pOutData[j++] = pInData[i];
		}
	}

	return j;
}

int XBeeApiUnescapeData(BYTE *pInData, int len, BYTE *pOutData)
{
	int i, j;

	for(j = 0, i = 0; j < len; i++) {
		BYTE tmp = pInData[j++];

		if( tmp == ESCAPE_CHARACTER )
		{ // character was escaped
			tmp = pInData[j++] ^ (BYTE)0x20;
			pOutData[i] = tmp;
		} 
		else {  // normal character
			pOutData[i] = tmp;
		}
	}

	return i;
}

int XBeeApiAssembleCommand(XBEE_CMD cmd, BYTE *pData, int dataLen, BYTE *pOutData)
{
	int i = 0;
	BYTE *p;

	// add delimiter
	pOutData[i++] = FRAME_DELIMITER;

	// append checksum
	p = malloc(dataLen + 2 + 1 + 1); // +2 for length, +1 for command, +1 for checksum
	if(p == NULL)
		return 0;
	p[0] = (BYTE)(( (dataLen + 1) >> 8) & 0xFF);
	p[1] = (BYTE)(( (dataLen + 1) >> 0) & 0xFF);
	p[2] = (BYTE)cmd;
	memcpy(p + 3, pData, dataLen);
	p[dataLen + 3] = XBeeApiCalcChecksum(p + 2, dataLen + 1);

	// escape data
	i += XBeeApiEscapeData(p, dataLen + 2 + 1 + 1, pOutData + i);

	free(p);

	return i;
}

XBEE_CMD XBeeApiUnassembleCommand(BYTE *pData, int dataLen, BYTE *pOutData, int *pOutDataLen)
{
	BYTE *p;
	int tmp, tmpLen;
	XBEE_CMD cmd;

	if(pData == NULL || dataLen == 0)
		return XBEE_CMD_NONE;

	if(*pData != FRAME_DELIMITER)
		return XBEE_CMD_NONE;

	p = malloc(dataLen);
	if(p == NULL)
		return XBEE_CMD_NONE;

	tmpLen = XBeeApiUnescapeData(pData + 1, dataLen - 1, p);
	if(tmpLen < (2 + 1 + 1)) // there should be at least length, checksum and command present
		return XBEE_CMD_NONE;

	// check if datalen matches
	tmp = (p[0] << 8) + p[1];
	if((tmpLen - 3) != tmp)
		return XBEE_CMD_NONE;

	// now check if checksum is correct
	if(!XBeeApiCheckChecksum(p + 2, tmpLen - 2))
		return XBEE_CMD_NONE;

	// copy data to output buffer
	memcpy(pOutData, p + 3, tmpLen - 4);
	*pOutDataLen = tmpLen - 4;
	cmd = (XBEE_CMD)p[2];

	free(p);

	return cmd;
}
