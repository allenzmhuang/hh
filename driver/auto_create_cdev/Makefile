obj-m:=auto_create_cdev.o 
KNERNEL_DIR=/home/gec/exercise/linux-4.10.12
PWD=$(shell pwd)


modules:
	$(MAKE) -C $(KNERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KNERNEL_DIR) M=$(PWD) modules clean


