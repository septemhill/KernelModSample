KDIR		:= /lib/modules/$(shell uname -r)/build
PWD		:= $(shell pwd)
IOTEST		:= io_test

obj-m		:= ioctlS.o
ioctlS-y	:= main.o

.PHONY:	all io_test clean

all: io_test
	@$(MAKE) -C $(KDIR) M=$(PWD) modules

io_test:
	@$(CC) $(IOTEST).c -o $(IOTEST)
	
clean:
	@$(MAKE) -C $(KDIR) M=$(PWD) clean
	@$(RM) -rf $(IOTEST)

