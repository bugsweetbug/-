ifneq ($(KERNELRELEASE),)
obj-m := charDev.o
else
PWD := $(shell pwd)
KDIR := /lib/modules/$(shell uname -r)/build
all:
	make -C $(KDIR) M=$(PWD)
clean:
	rm -rf *.o *.ko *.mod.c *symvers *.c~ *m *.cmd *~ *.mod modules*
endif
