EMBEDDED 	= n
SOCKCLI		= udpcli

ifeq ($(EMBEDDED), y)
KDIR		?= /home/septemhill/ast_sdk_059/build/ast2300/kernel
else
KDIR		?= /lib/modules/$(shell uname -r)/build
endif

PWD		:= $(shell pwd)

obj-m		:= udpsocket.o
udpsocket-y	:= main.o

.PHONY:	all clean

all:
	@$(MAKE) -C $(KDIR) M=$(PWD) modules
	@$(CC) $(SOCKCLI).c
clean:
	@$(MAKE) -C $(KDIR) M=$(PWD) clean
	@$(RM) -rf a.out
