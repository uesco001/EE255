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


int set_rtmon(pid_t pid, struct timespec *C, struct timespec *T){
	
	return syscall(399, pid, C, T);
}

int cancel_rtmon(pid_t pid){
	return syscall(400,pid);
}

int print_rtmon(pid_t pid){
	return syscall(401,pid);
}
int wait_until_next_period(void){
	return syscall(402);
}


int main(int argc, char* argv[]){
	long i;
	int r;
	pid_t pid = getpid();
	struct timespec C,T;
	// print C T  of this process
	r = print_rtmon(pid);
	
	C.tv_sec = 0;
	C.tv_nsec = 0;
	T.tv_sec = 0;
	T.tv_nsec = 0;

	// set C T to 0 and set it to this process
	r = set_rtmon(pid,&C,&T);
	r = wait_until_next_period();
	// print C T of this proc
	r = print_rtmon(pid);
	// try to cancel this process even tho its 0
	r = cancel_rtmon(pid);	
	C.tv_sec = 11;
	C.tv_nsec = 1;
	T.tv_sec = 11;
	T.tv_nsec = 10;

	r = set_rtmon(pid,&C,&T);
	while(1){
	for(i = 0; i < 10000; i++); 
	r = wait_until_next_period();
	}	

return 0;
}
