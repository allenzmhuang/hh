#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/wait.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>


#ifdef PDEBUG
#define printk0(fmt,args...) printk(fmt,##args)
#else
#define printk0(fmt,args...) /*do nothing*/
#endif
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("allen");

static dev_t devnum;
static struct cdev  *wqcdev=NULL;
static struct device *wqdev=NULL;
static struct class *wqclass=NULL;
static unsigned long time_now=0;
wait_queue_head_t wqhead;

/*wait queue condition */
unsigned condition=0;


/*initialize file operation*/
static int wq_open(struct inode *inodes,struct file *files)
{
	printk0("open successfuly");
	return 0;
}

static ssize_t wq_read(struct file *pfile,char __user * buf, size_t len, loff_t * off)
{
	int rt=-1;

	/******go to sleep******/
	wait_event_interruptible(wqhead,condition!=0);
	condition=0;
	if(len!=4)
	{
		printk0("data len error\n");
		return -1;
	}
	/*go to sleep*/
	

	rt=copy_to_user(buf,&time_now,4);
	if(rt!=0)
	{
		printk0("copy_to_user () falied\n");
		return -1;
	}
	return len;
}

static int wq_close(struct inode *inodes,struct file *files)
{

	return 0;
}


static struct file_operations wqfop=
{
	.owner=THIS_MODULE,
	.open=wq_open,
	.read=wq_read,
	.release=wq_close,
};


/*initialize timer structure */
static struct timer_list timer;
void timer_count(unsigned long data)
{
	unsigned long *p=(unsigned long*)data;
	if( (*p)<60)
	{

		(*p)++;
	}
	else
	{
		*p=0;
	}

	if(condition==0)
	{
		condition=1;
	}
	/************wait up queue****************/
	wake_up_interruptible(&wqhead);

	printk0("data=%ld\n",*p);
	mod_timer(&timer,jiffies+3*HZ);

}






static int __init waitqueue_init(void)
{
	int rt=-1;
	timer.expires=jiffies+3*HZ;
	timer.function=timer_count;
	timer.data=(unsigned long)&time_now;
	/*initialize timer*/
	printk0("size=%d\n",sizeof(condition));
	init_timer(&timer);
	 
	/*device class create*/
	wqclass=class_create(THIS_MODULE,"wqclass");
	if (IS_ERR(wqclass))
	{
		rt=PTR_ERR(wqclass);
		printk0("class_create() falied\n");
		return -rt;
	}

	/*character device  create */
	if( !(wqcdev=cdev_alloc()) )
	{
		printk0("cdev_alloc() falied\n");
		rt=-1;
		goto cdev_alloc_failed;
	}


	/*initialize cdev */
	cdev_init(wqcdev,&wqfop);

	/*dynamic allocate a device's number */
	if( (rt=alloc_chrdev_region(&devnum,0,1,"wq_cdev")) <0)
	{
		printk0("alloc_chrdev_region()falied \n");
		goto alloc_chrdev_region_failed;
	}

	/*add cdev to the kernel*/
	if( ( rt=cdev_add(wqcdev,devnum,1) ) <0)
	{
		printk0("cdev_add()  falied\n");
		goto cdev_add_faled;
	}

	/*create device */
	wqdev=device_create(wqclass,NULL,devnum,NULL,"wq_device");
	if(IS_ERR(wqdev))
	{
		printk0("device_create() falied\n");
		rt=PTR_ERR(wqdev);
		goto cdev_add_faled;
	}

	/*initialize wait queue head*/
	init_waitqueue_head(&wqhead);

	/*add timer to kernel*/
	add_timer(&timer);

	printk0("init successfuly\n");
	return rt;
	/*error handling*/

cdev_add_faled:
	unregister_chrdev_region(devnum,1);
alloc_chrdev_region_failed:
	cdev_del(wqcdev);
cdev_alloc_failed:
	class_destroy(wqclass);


	return rt;
}

static void __exit waitqueue_exit(void)
{

	device_destroy(wqclass,devnum);
	class_destroy(wqclass);
	cdev_del(wqcdev);
	unregister_chrdev_region(devnum,1);
	del_timer(&timer);
	remove()
	printk0("exit successfuly");	
}


module_init(waitqueue_init);
module_exit(waitqueue_exit);

