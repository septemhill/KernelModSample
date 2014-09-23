KDIR		:= /lib/modules/$(shell uname -r)/build
PWD		:= $(shell pwd)

obj-m		:= wait.o
wait-y		:= main.o

.PHONY:	all clean

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

