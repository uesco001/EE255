#include <linux/kernel.h>
#include <linux/compat.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(cancel_rtmon,pid_t, pid)
{
	pid_t pid_tsk = 0;
	if(pid < 0){
		printk("non valid pid\n");
		return -1;
	}	
	struct task_struct *task;
	if(pid >0){
		pid_tsk = pid;
	}else{
		pid_tsk = current->pid;
	}
	task = find_task_by_pid_ns(pid_tsk, &init_pid_ns);
	if(task)
	{
	  bool C_isempty = task->C.tv_sec + task->C.tv_nsec == 0;
	  bool T_isempty = task->T.tv_sec + task->T.tv_nsec == 0;
	  if(C_isempty || T_isempty){
		printk("C and T are empty can't cancel\n");
		return -1;
	  }else {
  		task->C.tv_sec = 0;
		task->C.tv_nsec = 0;
		task->T.tv_sec = 0;
		task->T.tv_nsec = 0;
		return 0;
		}
	}
	else{
		printk("Not valid pid\n");
		return -1;
	}
}
