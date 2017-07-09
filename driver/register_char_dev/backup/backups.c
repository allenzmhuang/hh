/*
 *实验目的：
 *	熟悉字符设备驱动的编程流程，掌握相关api
 *
 *
 *
*/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>


/*定义一个字符设备*/
static struct cdev  *test_cdev=NULL;

/*定义一个字符设备的设备号*/
static unsigned int devnum;

/******文件操作相关api********/




/*设备的文件操作集*/

static struct file_operations test_file=
{
	/*
	.open=test_open,
	.read=test_read,
	.write=test_write
	.release=test_close
*/
};
 
static int __init chrdev_test_init(void)
{
	int rt;
	
	if((rt=alloc_chrdev_region(&devnum,1,1,"tets_drv"))<0)
	{
		printk("alloc_chrdev_region failed!\n");
		return -1;
	}
	
	if( (test_cdev=cdev_alloc()) ==NULL)
	{
		printk("cdev_alloc()  failed!\n");
		goto unregister_devnum;
	
	}
	
	cdev_init(test_cdev,&test_file);

	if( (rt=cdev_add(test_cdev,devnum,1)) <0)
	{
		printk("cdev_add failed!\n");
		goto cdev_del;
	}	
	
	printk("test_drv init success\n");
	return 0;


cdev_del:
	cdev_del(test_cdev);
	
unregister_devnum:
	unregister_chrdev_region(devnum,1);
	
	return -1;
}


static void __exit chrdev_test_exit(void)
{
	cdev_del(test_cdev);
	unregister_chrdev_region(devnum,1);
}


module_init(chrdev_test_init);
module_exit(chrdev_test_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("THIS MODULE IS A A CHAR DEV TETS");
MODULE_AUTHOR("A LUCKY GUY");

