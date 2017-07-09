obj-m :=modules_programing_format.o
KERN_DIR=/home/gec/exercise/linux-4.10.12
PWD=$(shell pwd)
modules:
	$(MAKE) -C $(KERN_DIR) M=$(PWD) modules
clean:
	rm  *.o *.mod.c *.order *.ko *.symvers
.PHONLY:clean