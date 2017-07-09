

/*
 *实验目的：
 *	熟悉字符设备驱动的编程流程，掌握相关api
 *
 *
 *
*/


	//设备号 在 fs.h

	/*动态申请*/
//	int alloc_chrdev_region(dev_t *devnum, unsigned baseminor, unsigned count , const char *name);
	/*静态申请*/
//	int register_chrdev_region(dev_t devnum, unsigned count, const char *name);	
	/*注销设备号*/
//	unregister_chrdev_region(dev_t num,int count);

 	//cdev 相关api 在 cdev.h
//	void cdev_init(struct cdev *dev, const struct file_operations *files);

//	struct cdev *cdev_alloc(void);

//	void cdev_put(struct cdev *p);

//	int cdev_add(struct cdev *dev, dev_t devnum, unsigned count);

//	void cdev_del(struct cdev *dev);

//	//fil_operations
	
	/*打开设备*/
//	init (*open)(struct inode * s_inode ,struct file *s_file);
	/*关闭设备*/
//	init (*release)(struct inode * s_inode ,struct file *s_file);
	/*从设备中读取数据*/
//	ssize_t (*read)(struct file* fs ,char __user *buf,size_t len ,loff_t *offset );
	/*向设备写入数据*/
//	ssize_t (*write)(struct file* fs ,const char __user *buf,size_t len ,loff_t *offset );
	/*通过命令参数控制设备*/
//	int  (*ioctl)(struct inode* inode_s ,struct file* fs ,unsigned cmd ,unsigend long passarg );
	
	/*从用户空间获取数据*/
//	int copy_from_user(void *to ,const void __user *from ,int n);

	/*给用户空间传递数据*/
//	int copy_to_user(void  __user *to ,const void *from ,int n);
	
/**/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

/*定义一个字符设备*/
static struct cdev  *test_cdev=NULL;

/*定义一个字符设备的设备号*/
static unsigned int devnum;

/******文件操作相关api********/

static int test_open(struct inode* inodes,struct file* s_file)
{

	printk("open success !\n");
	return 0;
}

static int test_close(struct inode* inodes,struct file* s_file)
{

	printk("close success !\n");
	return 0;
}

static ssize_t test_read(struct file* fs ,char __user *buf,size_t len ,loff_t *offset)
{

	char kbuf[10]="hello";
	int rt;
	if( (rt=copy_to_user(buf,kbuf,6)) <6);
	{
		printk("write failed !\n");
		return -1;
	}
	
	return 0;
}

static ssize_t test_write(struct file* fs ,const char __user *buf,size_t len ,loff_t *offset)
{

	char kbuf[10];
	int rt=-1; 
	
	if( (rt=copy_from_user(kbuf,buf,10)) <10);
	{
		printk("write failed !\n");
		return -1;
	}
	
	return rt;
}

/*设备的文件操作集*/

static struct file_operations test_file=
{
	 .owner		= THIS_MODULE,
	.open=test_open,
	.read=test_read,
	.write=test_write,
	.release=test_close,

};




static int __init chrdev_test_init(void)
{
	int rt;
	
	/*动态申请设备号*/
	if((rt=alloc_chrdev_region(&devnum,1,1,"register_test_cdev"))<0)
	{
		printk("alloc_chrdev_region failed!\n");
		return -1;
	}
	
	/*获取一个字符设备结构体*/
	if( (test_cdev=cdev_alloc()) ==NULL)
	{
		printk("cdev_alloc()  failed!\n");
		goto unregister_devnum;
	
	}
	
	//初始化字符设备
	cdev_init(test_cdev,&test_file);

	//添加字符设备到驱动中
	if( (rt=cdev_add(test_cdev,devnum,1)) <0)
	{
		printk("cdev_add failed!\n");
		goto cdev_del;
	}	
	
	printk("test_drv init success\n");
	return 0;


/*删除字符设备，释放内存*/
cdev_del:
	cdev_del(test_cdev);

/*注销设备号*/	
unregister_devnum:
	unregister_chrdev_region(devnum,1);
	
	return -1;
}



static void __exit chrdev_test_exit(void)
{
	cdev_del(test_cdev);
	unregister_chrdev_region(devnum,1);
	printk("exit success!\n");
}


/*驱动初始化*/
module_init(chrdev_test_init);
module_exit(chrdev_test_exit);

/*驱动相关信息*/
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("THIS MODULE IS A A CHAR DEV TETS");
MODULE_AUTHOR("A LUCKY GUY");

