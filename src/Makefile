# TID — The Instant Destroyer
# Author: Ahmad Qasim Mohammad Hassan
# ORCID:  0009-0001-4360-0802

obj-m += tid.o

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

EXTRA_CFLAGS += -O2 -Wall -Wextra -mclflushopt

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

install:
	sudo insmod tid.ko

uninstall:
	sudo rmmod tid
