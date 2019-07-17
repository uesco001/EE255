#define _GNU_SOURCE

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/compat.h>
#include <linux/syscalls.h>
#include <linux/pid.h>
#include <linux/proc_ns.h>
#include <linux/proc_fs.h>
#include <linux/sched/task.h>
#include <linux/pid_namespace.h>

SYSCALL_DEFINE4(set_rtmon,pid_t, pid, struct timespec *,C, struct timespec *,T, int,cpuid)
{
	if(cpuid < 0 || cpuid > 3)
	{ 
	    	printk("invalid CPU ID\n");
	}

	if(C == 0 || T == 0)
	{
		printk("Not valid pointer\n");
		return -1;
	}
	if(C->tv_sec > 10 || T->tv_sec > 10)
	{ 
		printk("Over 10 seconds\n");
		return -1;
	}	
	if((C->tv_sec == 0 && C->tv_nsec < 1000000) || (T->tv_sec == 0 && T->tv_nsec < 1000000))
	{ 
		printk("below 1ms\n");
		return -1;
	}
	
	pid_t pid_tsk = pid;
	if(pid > 0){
		pid_tsk = pid;
	}else if(pid == 0){
		pid_tsk = current->pid;
	}else{
		printk("invalid pid\n");
		return -1;
	}
			
	struct task_struct *task; 
	task = find_task_by_pid_ns(pid_tsk, &init_pid_ns);
	if(task)
	{
	  bool C_isempty = task->C.tv_sec + task->C.tv_nsec == 0;
	  bool T_isempty = task->T.tv_sec + task->T.tv_nsec == 0;
	  if(C_isempty || T_isempty){
		
		task->C.tv_sec = C->tv_sec;
		task->C.tv_nsec = C->tv_nsec;
		task->T.tv_sec = T->tv_sec;
		task->T.tv_nsec = T->tv_nsec;
	  }else {
		printk("Not empty\n");
		return -1;
	  }

	}
	else{
		printk("Not valid pid\n");
		return -1;
	}
	
	struct cpumask mask;
	cpumask_clear(&mask);
	cpumask_set_cpu(cpuid, &mask);
	if(!(sched_setaffinity(0, &mask)))
	{
		printk("unable to set affinity \n");
		return -1;
	}

	return 0;
}



