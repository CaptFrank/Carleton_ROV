/**********************************************************************
*
*    Author:		shondll (shondll@yahoo.com)
*    Company:		-
*    Date:			14/06/2010
*    File Version:	1.00
*    Description:	global include file
*
**********************************************************************/

/* Necessary includes for device drivers */
#include <linux/init.h>
//#include <linux/config.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/file.h>
#include <linux/tty.h>
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <asm/ioctls.h>


typedef unsigned char BYTE;

// simple predefines
#define malloc(size_in_bytes) kmalloc(size_in_bytes, GFP_KERNEL)
#define free kfree

#define printf printk

typedef enum _BOOL {
	FALSE = 0,
	TRUE = 1
} BOOL;

#include "SerialPortFuncs.h"
#include "XBeeSupportFuncs.h"
#include "XBeeDriver.h"
#include "commandParser.h"





