#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

static unsigned devnum;

static struct cdev *atomiccdev=NULL;
static struct device *dev=NULL;
static struct class  *atomicclass=NULL;

/*****declare atomic variable and initialize*****/
atomic_t v;

static int atomic_open(struct inode *inodes, struct file *files)
{
	//decrease 1 and test result,if equal 0 (is false) reverse is true
	if(!atomic_dec_and_test(&v))
	{
		printk("this driver can open only once at the same time!\n");
		atomic_inc(&v);//increase 1
		return -EBUSY;
	}

	printk("open is successfuly\n");
	return 0;
}

static ssize_t atomic_write( struct file *pfile, const char __user * buf, size_t len, loff_t * off)
{
	int rt=-1;
	char kbuf[6];

	if(len!=6)
	{
		printk("len is too short\n");
		return -1;
	}
	rt=copy_from_user(kbuf,buf,len);
	/*if( (rt=copy_from_user(kbuf,buf,len)))
	{
		printk("copy_from_user() error\n");
		return -1;
	} */
	if(rt!=0)
	{
		printk("copy_from_user ( ) failed \n");
		return -1;
	}

	printk("%s\n",kbuf);
	return len;
}


static int atomic_release(struct inode *inodes, struct file *files)
{
	atomic_inc(&v);//increase 1
	printk("close is successfuly\n");
	return 0;
}
static struct file_operations atomicfop=
{
	.owner=THIS_MODULE,
	.open=atomic_open,
	.write=atomic_write,
	.release=atomic_release,
};


 
static int __init atomic_init(void)
{
	int rt=-1;
	printk("init_start\n");
	
	/*********initialize atomic*************/
	atomic_set(&v,1);

	atomicclass=class_create(THIS_MODULE,"atomicclass");
	if(IS_ERR(atomicclass))
	{
			rt=PTR_ERR(atomicclass);
			printk("class_craete() failed\n");
			return rt;
	}

	if( (atomiccdev=cdev_alloc()) ==NULL)
	{
			printk("cdev_alloc() failed\n");
			rt=-1;
			goto cdev_alloc_faiatomic;
	}

	cdev_init(atomiccdev,&atomicfop);
	if( (rt=alloc_chrdev_region(&devnum,0,1,"atomiccdev")) <0)
	{
		printk("alloc_chrdev_region() failed\n");
		goto alloc_chrdev_region_faiatomic;
	}

	if ( (rt = cdev_add(atomiccdev,devnum,1)) <0)
	{
		printk("cdev_add() faiatomic\n");
		goto cdev_add_faiatomic;
	}

	dev=device_create(atomicclass,NULL,devnum,NULL,"atomic_device%d",0);
	if (IS_ERR(dev))
	{
		printk("device_create() failed\n");
		rt=PTR_ERR(dev);
		goto cdev_add_faiatomic;
	}
printk("init_successfuly\n");
	return rt;
cdev_add_faiatomic:
	unregister_chrdev_region(devnum,1);
alloc_chrdev_region_faiatomic:
	cdev_del(atomiccdev);
cdev_alloc_faiatomic:
	class_destroy(atomicclass);
printk("init_faiatomic\n");
	return  rt;
}

static void __exit atomic_exit(void)
{
	device_destroy(atomicclass,devnum);
	class_destroy(atomicclass);
	cdev_del(atomiccdev);
	unregister_chrdev_region(devnum,1);
	printk("uninstall_successfuly\n");
}



module_init(atomic_init);
module_exit(atomic_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("HZM");
