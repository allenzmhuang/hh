#include <linux/init.h> //init 
#include <linux/kernel.h> 
#include <linux/module.h> // module 
#include <linux/fs.h>	// struct file ,file_operations ,inode ,syscall interface and so  on 
#include <linux/cdev.h>	// struct cdev, operation cdev api (fs/_dev.c)
//#include <linux/uaccess.h> 
#include <linux/device.h> //class header file (which to implement these api: ---->/drivers/base/class.c)
#include <asm/uaccess.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#define PDEBUG
#ifdef PDEBUG
#define printk0(fmt,args...)  printk(KERN_EMERG"info:"fmt,##args)
#else
#define printk0(fmt,args...)  /*do nothing*/ 
#endif

/*
*
*
*more API's implement infomation and define variable located in these file :cdev.h ,error.h,fs.h
*core.c,device.h,class.h ,init.h kernel.h module.h .
*
*/
// declare driver's variable: dev_t,cdev ,class,device

static dev_t devnum;

static struct cdev *testcdev;

static struct class *testcdev_class=NULL;

struct device *dev_test=NULL;

/*implement file_operations member function*/
static int test_open(struct inode *ionde, struct file *file )
{
	printk0("open devtest successfuly\n");
	return 0;
}

static ssize_t  test_read(struct file *pfile,char __user * buf, size_t len, loff_t * off)
{
	int rt=-1;
	char kbuf[7]="yesmom";
	if(len<7)
	{
			printk0("len no equal 7\n");
			return -1;
	}
	rt=copy_to_user(buf,kbuf,7);
	printk0("rt=%d\n",rt); 
	if(rt!=0)
	{
		printk0("copy_to_user failed \n");
		return -1;
	}

	return rt;

}

static ssize_t  test_write(struct file *pfile, const char __user * buf, size_t len, loff_t * off)
{
	int rt=-1;
	char kbuf[6];
	/*fist must check len */
	if(len!=6)
	{
		printk0("len no equal 6\n");
		return -1;
	}

	rt=copy_from_user(kbuf,(char*)buf,6);
	printk0("rt=%d\n",rt); 
	if(rt!=0)
	{
		printk0("copy_to_user failed \n");
		return -1;
	}
	printk0("kbuf=%s",kbuf);
	return rt;

}

/*initialize file_operations*/
static struct  file_operations testfile=
{
	.owner=THIS_MODULE,
	.open=test_open,
	.read=test_read,
	.write=test_write,
};



static int __init  init_auto_create_cdev(void)
{

	int rt=0;
/*
 *IS_ERR: check address is legal(we need a valid memory address )
 *if you want to know error mask ,use PTR_ERR() to get it,please
 *if create class successfuly  ,cd sys/class/ you will see a directory name xxxx 
 *that which class you be named
 *class_create(), device_create() e.g in core.c
 */	

	/*1:create driver device class	*/
	testcdev_class=class_create(THIS_MODULE,"mytestclass");
	if(IS_ERR(testcdev_class))
	{
		rt=PTR_ERR(testcdev_class);
		printk0("class_create failed \n");
	}
	/*2:allocate memory to the pointer testcdev*/
	if(NULL == (testcdev=cdev_alloc()) )
	{
		printk0("cdev_alloc failed \n");
		goto destroy_class;
	}
	/* allocte  device number  to the character device with dynamic */
	if((rt=alloc_chrdev_region(&devnum,0,1,"testcdev")) <0)
	{
		printk0("alloc alloc_cdev_region failed\n");
		goto del_cdev;
	}


	cdev_init(testcdev,&testfile);
	if( (rt=cdev_add(testcdev,devnum,1)) <0)
	{
		printk0("cdev_add failed \n");
		goto unregister_chrdev;
	}

	dev_test=device_create(testcdev_class, NULL,devnum, NULL,"testdriver");
	if(IS_ERR(dev_test))
	{
		rt=PTR_ERR(dev_test);
		printk0("device_create failed \n");
		goto unregister_chrdev;
	}

	return rt;


/*error handling:unregsister device's number,delete cdev destroy  device's class*/
unregister_chrdev:
	unregister_chrdev_region(devnum,1);	

del_cdev:
	cdev_del(testcdev);

destroy_class:
	class_destroy(testcdev_class);

	return rt;

}




static void __exit  exit_auto_create_cdev(void)
{
	/*handling when driver is uninstalled */
	device_destroy(testcdev_class,devnum);
	class_destroy(testcdev_class);
	cdev_del(testcdev);
	unregister_chrdev_region(devnum,1);
	printk0("testcdev  uninstall successfuly \n");
}



module_init(init_auto_create_cdev);
module_exit(exit_auto_create_cdev);

MODULE_AUTHOR("HZM");
MODULE_LICENSE("GPL v2");