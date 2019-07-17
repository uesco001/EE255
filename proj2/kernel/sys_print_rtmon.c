#include <linux/kernel.h>
#include <linux/compat.h>
#include <linux/syscalls.h>
#include <linux/sched/signal.h>
SYSCALL_DEFINE1(print_rtmon,pid_t, pid)
{
	pid_t pid_tsk = 0;
	if(pid >= 0){
	 	if(pid == 0){
			pid_tsk = current->pid;
	   	}else{
			 pid_tsk = pid;
        	}

		struct task_struct *task;
		task = find_task_by_pid_ns(pid_tsk, &init_pid_ns);
		if(task)
		{
		  unsigned long C = (task->C.tv_sec)*1000 + (task->C.tv_nsec)/(1000000);
		  unsigned long T  = (task->T.tv_sec)*1000 + (task->T.tv_nsec)/(1000000);
		  printk("PID %d, C %lu ms, T %lu ms\n",pid,C,T);	
		  return 0;
		}
		else{
			printk("Not valid pid\n");
			return -1;
		}
	}else if(pid < -1){
	
		printk("invalid pid\n");
		return -1;
	}else{
		struct task_struct *p,*t;
		unsigned long C;
		unsigned long T;
		read_lock(&tasklist_lock);
		for_each_process_thread(p,t){
		 
			C = (t->C.tv_sec) + (t->C.tv_nsec);
		  	T = (t->T.tv_sec) + (t->T.tv_nsec);
			if(C != 0 && T != 0){
		 	C = (t->C.tv_sec)*1000 + (t->C.tv_nsec)/(1000000);
		  	T  = (t->T.tv_sec)*1000 + (t->T.tv_nsec)/(1000000);
			
		 	 printk("PID %d, C %lu ms, T %lu ms\n",t->pid,C,T);	
			}
		}
		read_unlock(&tasklist_lock);

		return 0;
	

	}
	return 0;
}
