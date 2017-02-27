# This is taken straight from Documentation/kprobes.txt

obj-m := my_do_execve.o
obj-m += my_context_switch_jprobe.o 

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	rm -f *.mod.c *.ko *.o
