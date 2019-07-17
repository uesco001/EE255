#define _GNU_SOURCE
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

int main(int argc, char* argv[]){
	
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
