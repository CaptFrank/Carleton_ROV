/**********************************************************************
*
*    Author:		shondll (shondll@yahoo.com)
*    Company:		-
*    Date:			14/06/2010
*    File Version:	1.00
*    Description:	Support functions for the serial controller
*
**********************************************************************/

#ifndef _SERIAL_PORT_FUNCS_H
#define _SERIAL_PORT_FUNCS_H

int SpOpen(void);
int SpClose(void);

int SpWrite(BYTE *pData, int count);
int SpRead(BYTE *pData, int count);

int SpOpened(void);

#endif // _SERIAL_PORT_FUNCS_H

