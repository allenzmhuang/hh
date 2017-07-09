obj-m :=modules_programing_format.o
KERN_DIR=/home/gec/exercise/linux-4.10.12
PWD=$(shell pwd)
all:
	$(MAKE) -C $(KERN_DIR) M=$(PWD) all
clean:
	rm  *.o *.mod.c *.order *.ko *.symvers
.PHONLY:clean