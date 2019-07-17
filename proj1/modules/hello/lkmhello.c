#include <linux/init.h>
#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */

MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");





static int __init lkmhello_init(void)
{
   printk("Hello world! team05 in kernel space\n");
   return 0;
}


static void __exit lkmhello_exit(void)
{
  printk("Good Prof. Kim :^(\n");
}  

module_init(lkmhello_init);
module_exit(lkmhello_exit);
