#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/semaphore.h>

static unsigned devnum;
static struct cdev *semaphorecdev=NULL;
static struct device *dev=NULL;
static struct class  *semaphoreclass=NULL;

/*declare struct semaphore variable*/

struct semaphore sema;




/*flage*/
int open_flage=1;

static int semaphore_open(struct inode *inodes, struct file *files)
{
	
	/*get semaphore */

	
	if(down_interruptible(&sema))
	{
		printk("INTERRUPT MY SLEEP \n");
		return -EINTR;
	}
	
	printk("open is successfuly\n");
	return 0;
}

static ssize_t semaphore_write( struct file *pfile, const char __user * buf, size_t len, loff_t * off)
{
	int rt=-1;
	char kbuf[6];

	if(len!=6)
	{
		printk("len is too short\n");
		return -1;
	}
	rt=copy_from_user(kbuf,buf,len);
	if(rt!=0)
	{
		printk("copy_from_user ( ) failed \n");
		return -1;
	}

	printk("%s\n",kbuf);
	return len;
}


static int semaphore_release_close(struct inode *inodes, struct file *files)
{
	/*up semaphore*/
	up(&sema);

	printk("close is successfuly\n");
	return 0;
}

static struct file_operations semaphorefop=
{
	.owner=THIS_MODULE,
	.open=semaphore_open,
	.write=semaphore_write,
	.release=semaphore_release_close,
};


 
static int __init semaphore_init(void)
{
	int rt=-1;
	printk("init_start\n");
	


	semaphoreclass=class_create(THIS_MODULE,"semaphoreclass");
	if(IS_ERR(semaphoreclass))
	{
			rt=PTR_ERR(semaphoreclass);
			printk("class_craete() failed\n");
			return rt;
	}

	if( (semaphorecdev=cdev_alloc()) ==NULL)
	{
			printk("cdev_alloc() failed\n");
			rt=-1;
			goto cdev_alloc_faisemaphore;
	}

	cdev_init(semaphorecdev,&semaphorefop);
	if( (rt=alloc_chrdev_region(&devnum,0,1,"semaphorecdev")) <0)
	{
		printk("alloc_chrdev_region() failed\n");
		goto alloc_chrdev_region_faisemaphore;
	}

	if ( (rt = cdev_add(semaphorecdev,devnum,1)) <0)
	{
		printk("cdev_add() faisemaphore\n");
		goto cdev_add_faisemaphore;
	}

	dev=device_create(semaphoreclass,NULL,devnum,NULL,"semaphore_device%d",0);
	if (IS_ERR(dev))
	{
		printk("device_create() failed\n");
		rt=PTR_ERR(dev);
		goto cdev_add_faisemaphore;
	}

	/*initialize semaphore */
	sema_init(&sema,1);
	printk("init_successfuly\n");


	return rt;
cdev_add_faisemaphore:
	unregister_chrdev_region(devnum,1);
alloc_chrdev_region_faisemaphore:
	cdev_del(semaphorecdev);
cdev_alloc_faisemaphore:
	class_destroy(semaphoreclass);
printk("init_faisemaphore\n");
	return  rt;
}

static void __exit semaphore_exit(void)
{
	device_destroy(semaphoreclass,devnum);
	class_destroy(semaphoreclass);
	cdev_del(semaphorecdev);
	unregister_chrdev_region(devnum,1);
	printk("uninstall_successfuly\n");
}



module_init(semaphore_init);
module_exit(semaphore_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("HZM");
