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
#include <linux/sched/signal.h>




bool RT_test(unsigned int R_old, unsigned int Ctime[],unsigned int Ttime[], unsigned int size, unsigned C_low, unsigned T_low)
{

	unsigned int R_new = 0;
	unsigned i;
	for(i = 0; i < size; i++){
	   R_new += Ctime[i]*( R_old/Ttime[i] );
	}
	R_new += C_low;
	if(R_new > T_low){ return false;}
	if(R_new ==  R_old)
	{
		if(T_low > R_new)
		{
			return true;
		}else{
			return false;
		}
	}else{
		RT_test(R_new,Ctime, Ttime, size, C_low,T_low);
	 }
		

}



SYSCALL_DEFINE4(set_rtmon,pid_t, pid, struct timespec *,C, struct timespec *,T, int,cpuid)
{
	if(cpuid < 0 || cpuid > 3)
	{ 
	    	printk("invalid CPU ID\n");
		return -1;
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

// declare the C and T arrays
	unsigned int C_array[100];
	unsigned int T_array[100];
	unsigned size = 0;

	unsigned long CC;  // store C time
	unsigned long TT; // store T time
	unsigned int C_low = 0;
	unsigned int T_low = 0;

// being searching throuhg proceess and threads	
	struct task_struct *p,*t;
	read_lock(&tasklist_lock);
	for_each_process_thread(p,t){
	   	if(t->cpu_i == cpuid){
			CC = (t->C.tv_sec) + (t->C.tv_nsec);
		  	TT = (t->T.tv_sec) + (t->T.tv_nsec);
			if(CC != 0 && TT != 0){
	         	 	CC = (t->C.tv_sec)*1000 + (t->C.tv_nsec)/(1000000);
		        	TT  = (t->T.tv_sec)*1000 + (t->T.tv_nsec)/(1000000);
				C_array[size] = CC;
				T_array[size] = TT;
				size++;
		        	 printk("PID %d, C %lu ms, T %lu ms, Size %d \n",t->pid,CC,TT,size);	

			}
		}
	}
	read_unlock(&tasklist_lock);
// end the search of process and threads


// start the search for lowest priority 
	if(size == 0){
		goto skip;
	}
	T_array[size] = T->tv_sec*1000 + T->tv_nsec/1000000;
	
	C_array[size] = C->tv_sec*1000 + C->tv_nsec/1000000;
	size = size + 1;
	int lowest_location = -1;
	unsigned j = 0;
	printk("before\n");
	for(j = 0; j < size; j++){
	printk("C = %d, T = %d\n",C_array[j],T_array[j]);

	}
	
	
	for(j = 0; j< size; j++){
	   if(T_low < T_array[j]){

		T_low = T_array[j];
		C_low = C_array[j];
		lowest_location = j;

	   }else if(T_low == T_array[j]){

		  if(C_low > C_array[j]){
		   T_low = T_array[j];
	           C_low = C_array[j];
		   lowest_location = j;
		   }
	   }
	}
// end search for lowest prirority 

// starting to move array 
	if(lowest_location != -1){
	for(j = lowest_location; j < size; j++){
		C_array[j] = C_array[j+1];
		T_array[j] = T_array[j+1];
	}
	size = size -1;
	}
// end moving array 	
	
	printk("after:\n");
	for(j = 0; j < size; j++){
	printk("C = %d, T = %d\n",C_array[j],T_array[j]);

	}
// start of Rate mono schedubality test
	
	if(RT_test(C_low,C_array, T_array, size, C_low, T_low)){
		printk("ITS SCHEDULABLE\n");
	}else{
		printk("its not sched\n");
		return -1;
	}




        skip:;
///  staring task search
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
	if( sched_setaffinity(pid, &mask) == -1 )
	{
		printk("unable to set affinity \n");
		return -1;
	}

	return 0;
}



