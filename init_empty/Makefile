ifeq ($(EMBEDDED),y)
KDIR		:= /home/septemhill/bmcprj/build/ast2300/kernel
else
KDIR		:= /lib/modules/$(shell uname -r)/build
endif

PWD		:= $(shell pwd)

obj-m		:= empty.o
empty-y		:= main.o

.PHONY:	all clean

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

