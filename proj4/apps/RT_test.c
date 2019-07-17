#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <syscall.h>
#include <sched.h>
#include <stdbool.h>
int set_rtmon(pid_t pid, struct timespec *C, struct timespec *T, int cpuid){
	return syscall(399, pid, C, T, cpuid);
}

bool RT_test(unsigned int R_old, unsigned int Ctime[],unsigned int Ttime[], unsigned int size, unsigned C_low, unsigned T_low)
{

	unsigned int R_new = 0;
	for(unsigned i = 0; i < size; i++){
	   R_new += Ctime[i]*ceil( R_old/Ttime[i] );
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

int main(int argc, char* argv[]){
	
	
	unsigned int C_array[] = {80, 40,  230,  100, 39 };
	unsigned int T_array[] = {100, 150, 400, 200, 100};
	unsigned int C_low = 0;
	unsigned int T_low = 0;

	unsigned int size = 4;
	unsigned int lowest_location = -1;
	unsigned i;	
	for(i = 0; i <= size; i++)
	{
		printf("C = %d, T = %d\n",C_array[i],T_array[i]);
	}

	printf("\n\n");

	unsigned j;
	for(j = 0; j <= size; j++){
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


	if(lowest_location != -1){
	for(i = lowest_location; i < size; i++){
		C_array[i] = C_array[i+1];
		T_array[i] = T_array[i+1];
	}
	}
	size--;
	for( i = 0; i <= size; i++)
	{
		printf("C = %d, T = %d\n",C_array[i],T_array[i]);
	}


	
	if(RT_test(C_low, C_array, T_array, size, C_low, T_low)){
		printf("yayyys\n");
	}else{
		printf("boooo\n");
	}
	



	int r;
	pid_t pid;
	struct timespec C,T;
	
	if(argc>5){
		printf("Too many arguments\n");
		return -1;
	}else if(argc<5){
		printf("Too little arguments\n");
		return -1;
	}else{
	
	char *pid_num = argv[1];
	char *c_ms = argv[2];
	char *t_ms = argv[3];
	char *cpu_num = argv[4];
	
	unsigned int pidnum = atoi(pid_num);
	unsigned long C_ms = atoi(c_ms);
	unsigned long T_ms = atoi(t_ms);
	unsigned int cpu_number = atoi(cpu_num);
	
	C.tv_sec = C_ms/1000;
	C.tv_nsec = C_ms*1000000;
	T.tv_sec = T_ms/1000;
	T.tv_nsec = T_ms*1000000;
	bool passed = set_rtmon(pidnum,&C,&T,cpu_number) == 0;	
	if(passed){
		printf("passed\n");
	}else{
		printf("failed\n");
	}
		
	printf("PID: %d, C: %lu, T: %lu, CPUID: %d\n", getpid(),C_ms,T_ms,cpu_number);	


       }	
	while(1);

return 0;
}
