# This Makefile has been simplified as much as possible.

# The headers folder
INCLUDEFOLDER := /home/shondll/EP9302/4.1.1-920t/include/
BINFOLDER := /home/shondll/EP9302/4.1.1-920t/bin/

all: main.c XBeeSupport.c XBeeSupport.h
	@echo compliling...
	$(BINFOLDER)arm-linux-gcc -I$(INCLUDEFOLDER) -I. -o XBeeSampleARM main.c XBeeSupport.c
	@echo done.

clean:
	@echo cleaning...
	rm -f *.o
	rm -f XBeeSample
	@echo done.


