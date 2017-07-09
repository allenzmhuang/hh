obj-m:=memdev.o
KERNEL_DIR=/home/gec/exercise/linux-4.10.12
PWD=$(shell pwd)
modules:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	rm *.o  *.mod.c  *.order *.ko    *.symvers