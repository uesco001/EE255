#include <linux/kernel.h>
#include <linux/compat.h>
#include <linux/syscalls.h>
#include <linux/sched/signal.h>
#include <linux/hrtimer.h>
SYSCALL_DEFINE0(wait_until_next_period)
{
	unsigned long C_sec = current->C.tv_sec; 
	unsigned long C_nsec = current->C.tv_nsec;
	
	unsigned long T_sec = current->T.tv_sec; 
	unsigned long T_nsec = current->T.tv_nsec;
	if( C_sec + C_nsec  || T_sec + T_nsec)
	{ 	
		printk("do some peridoic things\n");
		// DO PERIDOIC THINGS
		ktime_t expires = T_sec * 100000000 + T_nsec - C_sec*1000000000 - C_nsec;
		u64 delta = 0;
		int rc;
		__set_current_state(TASK_INTERRUPTIBLE);
		rc = schedule_hrtimeout_range(&expires, delta,HRTIMER_MODE_REL);		
		if(rc == 0){
		return 0;
		}else{
			printk("task was woken up\n");
			return 0;
		}
	}else{
		printk("C and T are still zero can make wait untill next period\n");
		return -1;
	}
	return -1;
}
