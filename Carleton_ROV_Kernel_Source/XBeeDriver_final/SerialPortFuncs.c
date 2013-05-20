/**********************************************************************
*
*    Author:		shondll (shondll_yahoo.com)
*    Company:		-
*    Date:		14/06/2010
*    File Version:	1.00
*    Description:	Support functions for the serial controller
*
**********************************************************************/
#include "includes.h"

// uncomment this line to not use the real serial port file, but a fake one
//#define VIRTUAL_PORT_MODE

/* DEFINE LOCAL TYPES HERE */

/* DEFINE LOCAL CONSTANTS HERE */


#define INVALID_HANDLE_VALUE NULL
#define SERIAL_DEVICE_FILENAME "/dev/ttyO3"
//#define SERIAL_DEVICE_FILENAME "/home/shondll/temp1"

/* DEFINE LOCAL VARIABLES HERE */
static struct file* TheSerialPortHandle = INVALID_HANDLE_VALUE;
static mm_segment_t old_fs = KERNEL_DS;

/* DECLARE LOCAL FUNCTIONS HERE */

/* DECLARE LOCAL FUNCTIONS HERE */

/* DEFINE FUNCTIONS HERE */

int SpOpen(void) 
{
#ifdef VIRTUAL_PORT_MODE
	TheSerialPortHandle = 1; // put some value to mark open
	return 0;
#else
	struct ktermios settings;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	// open tty, note that O_NDELAY is NOT set, didn't read anything with it set!
	TheSerialPortHandle = filp_open(SERIAL_DEVICE_FILENAME, O_RDWR | O_NOCTTY, 0);

	/* SET tty to 9600-8N1 */

	// copy settings for modification
	settings = *(((struct tty_struct*)TheSerialPortHandle->private_data)->termios);

	// setup baudrate
	settings.c_cflag &= ~CBAUD;
	settings.c_cflag |= B9600;

	// these have to always enabled
	settings.c_cflag |= (CLOCAL | CREAD);
	
	// disable parity
	settings.c_cflag &= ~PARENB;

	// one stop bit
	settings.c_cflag &= ~CSTOPB;

	// data size set to 8bits
	settings.c_cflag &= ~CSIZE;
	settings.c_cflag |= CS8;

	// disable flow control
	settings.c_cflag &= ~CRTSCTS;
	
	settings.c_iflag = 1;
	settings.c_oflag = 1;
	settings.c_lflag = 0;

	settings.c_cc[VMIN] = 0;
	settings.c_cc[VTIME] = 1; // 0.1s

	// apply changes to settings
	*(((struct tty_struct*)TheSerialPortHandle->private_data)->termios) = settings;

	set_fs(old_fs);
	if(IS_ERR(TheSerialPortHandle)) {
		return -1;
	}
	
	return 0;
#endif
}

int SpClose(void)
{
#ifdef VIRTUAL_PORT_MODE
	TheSerialPortHandle = INVALID_HANDLE_VALUE;
	return 0;
#else
	if( SpOpened() ) {
		old_fs = get_fs();
		set_fs(KERNEL_DS);		filp_close(TheSerialPortHandle, NULL);
		set_fs(old_fs);

		TheSerialPortHandle = INVALID_HANDLE_VALUE;
		return 0;
	}

	return -1;
#endif
}

int SpWrite(BYTE *pData, int count)
{
#ifdef VIRTUAL_PORT_MODE
	return count;
#else
	int written = 0;
	unsigned long long offset = 0;

	if( SpOpened() ) {
		old_fs = get_fs();
		set_fs(KERNEL_DS);

		written = vfs_write(TheSerialPortHandle, pData, count, &offset);

		set_fs(old_fs);
	}

	return written;
#endif
}

int SpRead(BYTE *pData, int count)
{
#ifdef VIRTUAL_PORT_MODE
	return 0;
#else
	int read = 0;
	unsigned long long offset = 0;

	if( SpOpened() ) {
		old_fs = get_fs();
		set_fs(KERNEL_DS);

		read = vfs_read(TheSerialPortHandle, pData, count, &offset);

		set_fs(old_fs);
	}

	return read;
#endif
}

int SpOpened(void)
{
	return (TheSerialPortHandle != INVALID_HANDLE_VALUE);
}
