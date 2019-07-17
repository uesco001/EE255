#include <linux/kernel.h>
#include <linux/compat.h>
#include <linux/syscalls.h>
#include <linux/sched/signal.h>
#include <linux/pid.h>
#include <linux/pid_namespace.h>
#include <linux/mm_types.h>
SYSCALL_DEFINE1(show_segment_info, int,pid)
{
	
	pid_t thepid;
	if(pid)
	{
	  thepid = pid;
	}else{
		thepid = current->pid;
	}
	struct task_struct *thestruct;
	struct mm_struct *themm;
	thestruct = find_task_by_pid_ns(thepid,&init_pid_ns);
	if( !(thestruct) )
	{
		printk("can't get struct\nnow exiting\n");
		return -1;
	}else
	{
	 	themm = thestruct->active_mm;
	}
	
	

	printk("[Memory segment address of process %d]\n", thepid);
	printk("%lx- %lx: code segment\n",themm->start_code,themm->end_code);
		
	printk("%lx - %lx: data segment\n",themm->start_data,themm->end_data);
	
	printk("%lx - %lx: heap segment\n",themm->start_brk,themm->brk);
	


	return 0;
}
