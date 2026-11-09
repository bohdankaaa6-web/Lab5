ifneq ($(KERNELRELEASE),)
# kbuild part of makefile

# Include path for the inc directory
ccflags-y += -I$(src)/inc

# Build two separate modules
obj-m := hello1.o hello2.o

else
# normal makefile
KDIR ?= /home/bohdana/repos/linux-stable

default:
	$(MAKE) -C $(KDIR) M=$$PWD

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
endif
