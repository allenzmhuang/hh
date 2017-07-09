#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>

static unsigned int devnum; 

static struct *cdev test_cdev=NULL;



static struct file_operations test_file=
{

};

static int __init register_char_test_init(void)
{
	int rt=alloc_chrdev_region(&devnum,1,1,"hzm_test");
	if(rt<0)
	{
		printk("alloc_chrdev_region  failed!\n");
		return -1;
	}

	test_cdev=cdev_alloc();
	if(test_cdev==NULL)
	{
		printk("cdev_alloc  failed!\n");
		goto unregister;
	}

	cdev_init(test_cdev, &test_file);

	rt=cdev_add(test_cdev, devnum, 1);
	if(rt<0)
	{
		printk("cdev_add faile!\n");
		goto cdev_del;
		
	}

	printk("register_char_dev_test is success \n");
	return 0;

cdev_del:
		cdev_del(test_cdev);
unregister:
		unregister_chrdev_region(devnum,1);

	return -1;

}

static void __exit register_char_test_exit(void)
{
	unregister_chrdev_region(devnum,1);
	cdev_del(test_cdev);
	printk("uninstall module success\n");
}

module_init(register_char_test_init);
module_exit(register_char_test_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRICPTION("THIS MODULE IS A REGISTER CHAR DEV TEST");
MODULE_AUTHOR("HZM");