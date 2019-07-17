

#ifndef _syscalls_set_cancel_print

#define _syscalls_set_cancel_print


int set_rtmon(pid_t pid, struct timespec *C, struct timespec *T){
	
	return syscall(399, pid, C, T);
}

int cancel_rtmon(pid_t pid){
	return syscall(400,pid);
}

int print_rtmon(pid_t pid){
	return syscall(401,pid);
}

#endif
