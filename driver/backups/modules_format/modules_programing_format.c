#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


static int __init helloworld_init(void)
{

	printk(KERN_EMERG "log_level:\n",KERN_EMERG);
	return 0;
}


static void __exit helloworld_exit(void)
{
	printk(KERN_EMERG "Here I am: \n"); 
}

module_init(helloworld_init);
module_exit(helloworld_exit);
MODULE_LICENSE("GPL");


