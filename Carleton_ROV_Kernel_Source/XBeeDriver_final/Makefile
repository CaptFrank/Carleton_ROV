# This Makefile has been simplified as much as possible.

# The headers folder
#INCLUDEFOLDER := /lib/modules/$(shell uname -r)/build/include/
INCLUDEFOLDER := /usr/src/linux-source-2.6.26/

XBeeDrv-objs := commandParser.o SerialPortFuncs.o XBeeDriver.o XBeeSupportFuncs.o
obj-m := XBeeDrv.o

all:
	make  -C $(INCLUDEFOLDER) M=$(shell pwd) modules	

clean:
	rm -f *.o *~ core *.d .*.cmd
	rm -f .tmp_versions/*
	rmdir .tmp_versions

