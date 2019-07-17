#include <linux/kernel.h>
#include <linux/compat.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE4(calc, int, param1, int, param2, char, operation, int*, result)
{
	printk("This confirms the sys_call team05 is invoked\n");
	if(result == 0)
	{
		printk("Bad pointer >:^(\n");
		return -1;
	}	
	if(operation == '-')
	{*result = param1 - param2;}

	else if(operation == '+')
	{
		*result = param1 + param2;
	}
	else if(operation == '/')
	{
	    if(param2){*result = param1/param2;}
	    else{ return -1;}
	}

	else if(operation == '*')
	{
		*result = param1 * param2;
	}
	else
	{
		return -1;
	}
	return 0;

}
