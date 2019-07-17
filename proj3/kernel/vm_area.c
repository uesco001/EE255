#include <linux/kernel.h>
#include <linux/compat.h>
#include <linux/syscalls.h>
#include <linux/sched/signal.h>
#include <linux/pid.h>
#include <linux/pid_namespace.h>
#include <linux/mm_types.h>
SYSCALL_DEFINE1(show_vm_area, int,pid)
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
	struct vm_area_struct *the_vm_area_struct = themm->mmap;	
	struct vm_area_struct *current_area = the_vm_area_struct;	
	
	printk("[Memory-mappped areas of process %d]\n", thepid);
	do{
		if(current_area->vm_flags & VM_LOCKED){
		printk("%lx-%lx: %lu bytes  [L]\n",current_area->vm_start,current_area->vm_end,current_area->vm_end - current_area->vm_start);
		}else{	  	
		printk("%lx-%lx: %lu bytes\n",current_area->vm_start,current_area->vm_end,current_area->vm_end - current_area->vm_start);
		}
		current_area = current_area->vm_next;
	}while(current_area != NULL);


	return 0;
}
