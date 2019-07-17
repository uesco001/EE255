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


int main(int argc, char* argv[]){
	
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
	// print C T of this proc
	r = print_rtmon(pid);
	// try to cancel this process even tho its 0
	r = cancel_rtmon(pid);	
	C.tv_sec = 11;
	C.tv_nsec = 1;
	T.tv_sec = 11;
	T.tv_nsec = 10;
	// try to set C over 10 
	r = set_rtmon(pid,&C,&T);
	// print to see if there is nothing
	r = print_rtmon(pid);
	C.tv_sec = 1;
	C.tv_nsec = 50000000;
	T.tv_sec = 2;
	T.tv_nsec = 250000000;
	// set it to something that it can be
	r= set_rtmon(pid,&C,&T); 
	// print it 	
	r = print_rtmon(pid);
	// remove then print 
	r = cancel_rtmon(pid);
	r = print_rtmon(pid);

	

return 0;
}
