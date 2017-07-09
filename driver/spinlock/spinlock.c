#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/spinlock.h>
static unsigned devnum;

static struct cdev *spincdev=NULL;
static struct device *dev=NULL;
static struct class  *spinclass=NULL;

/*declare spinlock_t variable*/

spinlock_t lock;



/*flage*/
int open_flage=1;

static int spin_open(struct inode *inodes, struct file *files)
{
	
	/*get spin lock*/
	spin_lock(&lock);

	if(--open_flage!=0)
	{
		printk("open failed:this driver can be open only onec at the same\n");
		open_flage++;
		spin_unlock(&lock);
		return -EBUSY;
	}
	
	/*unlock spin lock */
	/*note:you must be increase open_flage onec when you close */
	spin_unlock(&lock);

	printk("open is successfuly\n");
	return 0;
}

static ssize_t spin_write( struct file *pfile, const char __user * buf, size_t len, loff_t * off)
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


static int spin_release_close(struct inode *inodes, struct file *files)
{
	/*increase open_flage*/
	open_flage++;
	printk("close is successfuly\n");
	return 0;
}

static struct file_operations spinfop=
{
	.owner=THIS_MODULE,
	.open=spin_open,
	.write=spin_write,
	.release=spin_release_close,
};


 
static int __init spin_init(void)
{
	int rt=-1;
	printk("init_start\n");
	


	spinclass=class_create(THIS_MODULE,"spinclass");
	if(IS_ERR(spinclass))
	{
			rt=PTR_ERR(spinclass);
			printk("class_craete() failed\n");
			return rt;
	}

	if( (spincdev=cdev_alloc()) ==NULL)
	{
			printk("cdev_alloc() failed\n");
			rt=-1;
			goto cdev_alloc_faispin;
	}

	cdev_init(spincdev,&spinfop);
	if( (rt=alloc_chrdev_region(&devnum,0,1,"spincdev")) <0)
	{
		printk("alloc_chrdev_region() failed\n");
		goto alloc_chrdev_region_faispin;
	}

	if ( (rt = cdev_add(spincdev,devnum,1)) <0)
	{
		printk("cdev_add() faispin\n");
		goto cdev_add_faispin;
	}

	dev=device_create(spinclass,NULL,devnum,NULL,"spin_device%d",0);
	if (IS_ERR(dev))
	{
		printk("device_create() failed\n");
		rt=PTR_ERR(dev);
		goto cdev_add_faispin;
	}

	/*initialize spin lock*/
	spin_lock_init(&lock);

	printk("init_successfuly\n");


	return rt;
cdev_add_faispin:
	unregister_chrdev_region(devnum,1);
alloc_chrdev_region_faispin:
	cdev_del(spincdev);
cdev_alloc_faispin:
	class_destroy(spinclass);
printk("init_faispin\n");
	return  rt;
}

static void __exit spin_exit(void)
{
	device_destroy(spinclass,devnum);
	class_destroy(spinclass);
	cdev_del(spincdev);
	unregister_chrdev_region(devnum,1);
	printk("uninstall_successfuly\n");
}



module_init(spin_init);
module_exit(spin_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("HZM");
