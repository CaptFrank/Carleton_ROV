/**********************************************************************
*
*    Author:		shondll (shondll_yahoo.com)
*    Company:		-
*    Date:			04/12/2010
*    File Version:	1.00
*    Description:	Simple demo project
*
**********************************************************************/
#include "XBeeSupport.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* DEFINE LOCAL TYPES HERE */

/* DEFINE LOCAL CONSTANTS HERE */
#define  XBEE_FILE "/dev/XBee0"

/* DEFINE LOCAL VARIABLES HERE */

/* DECLARE LOCAL FUNCTIONS HERE */

/* DECLARE LOCAL FUNCTIONS HERE */

/* DEFINE FUNCTIONS HERE */

int main(int argc, char *argv[])
{
	long counter;
	char str[128];
	int result;
	int srcAddr, msglen;

	printf("This is a simple demonstration program for the XBeeDriver!\n");
	printf("Note: make sure to have loaded the XBee driver!\n");
	

	if( XBeeSetShortAddr(XBEE_FILE, 0x0002) == 0)
		printf("Changed short address to 0x0002\n");
	else {
		printf("ERROR changing short address!\n");
		return -1;
	}

	counter = 100;
	while(1) {		result = XBeeSend16(XBEE_FILE, 0x0001, (unsigned char *)&counter, sizeof(counter));
		if(result == 0) {
			int retries = 3;

			printf("Sent msg %08X to 0x0001, waiting for answer...\n", counter);
			sleep(2);
			
			while(retries--) {
				result = XBeeRcv16(XBEE_FILE, &srcAddr, str, &msglen);
				if(result == 1) {
					int i;
	
					printf("Received msg from 0x%04X, length %d, ", srcAddr, msglen);
					for(i = 0; i < msglen; i++)
						printf("%02X", str[i]);
					printf("\n");

					counter++;
					break;
				}
				else if(result == 0)
					printf("No message was received!\n");
				else
					printf("Some error occured in receiving!\n");

				sleep(1);
			}
		}
		else {
			printf("Error sending msg to 0x0001, retrying in 5s...\n");
			sleep(5);
		}
	}

	return 0;
}
