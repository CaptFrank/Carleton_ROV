obj-m := mcp23008.o
KERNELDIR := /usr/src/kernel


PWD := $(shell pwd)

CC := gcc

default: driver

clean:
	rm -f *.ko *.o *.mod.c Module.symvers modules.order

driver:
	@echo "Building driver"
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	

