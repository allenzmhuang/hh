obj-m :=semaphore.o
KERNEL_DIR=/home/gec/exercise/linux-4.10.12
CURRET_PWD:=$(shell pwd)

modules:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURRET_PWD) 	modules
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(CURRET_PWD) modules clean
