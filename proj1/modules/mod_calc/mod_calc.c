#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/unistd.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>



MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

unsigned long **sys_call_table;
//EXPORT_SYMBOL(sys_call_table);


asmlinkage unsigned long (*og_sys_calc)(int,int,char,int*);

asmlinkage unsigned long mod_calc(int param1, int param2, char operation,int *result){

	if(result == 0)
	{
	  printk("Bad pointer >:^(\n");
	  return -1;
	}

	if( (operation == '-' || operation == '+' || operation == '*' || operation == '/') && (param2) )
	{
		*result = param1 % param2; 
		return 0;
	}else{
	    return -1;
	}
}
	  



static int __init mod_calc_init(void)
{
    sys_call_table = (unsigned long **)0x80010184;
    printk("loading pointer of og sys call\n");
   og_sys_calc= (void*)sys_call_table[__NR_calc];
    printk("loading pointer of new sys call\n");
   sys_call_table[__NR_calc]=(long*)mod_calc;
    return 0;
}

static void __exit mod_calc_exit(void)
{
    sys_call_table[__NR_calc]=(long*)og_sys_calc;
    printk("exiting wowo");
}  


module_init(mod_calc_init);
module_exit(mod_calc_exit);
