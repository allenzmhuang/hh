obj-m:=register_test_cdev.o
KERNEL_DIR=/home/gec/exercise/linux-4.10.12
PWD=$(shell pwd)
all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) all
clean:
	rm *.o *.mod *.order *.ko *.mod.c *.symver