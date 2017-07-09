obj-m +=waitqueue.o
KERNEL_DIR=/home/gec/exercise/linux-4.10.12
PWD=$(shell pwd)

modules:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) KCPPFLAGS+=-DPDEBUG modules
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules clean