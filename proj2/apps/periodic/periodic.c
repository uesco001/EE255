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

/*
 * More details on this code: http://2net.co.uk/tutorial/periodic_threads
 *
 * Modified for use in homework.
 */

struct periodic_info {
	int timer_fd;
	unsigned long long wakeups_missed;
};

int make_periodic(unsigned int period, struct periodic_info *info)
{
	int ret;
	unsigned int ns;
	unsigned int sec;
	int fd;
	struct itimerspec itval;

	/* Create the timer */
	fd = timerfd_create(CLOCK_MONOTONIC, 0);
	info->wakeups_missed = 0;
	info->timer_fd = fd;
	if (fd == -1)
		return fd;

	/* Make the timer periodic */
	sec = period / 1000000;
	ns = (period - (sec * 1000000)) * 1000;
	itval.it_interval.tv_sec = sec;
	itval.it_interval.tv_nsec = ns;
	itval.it_value.tv_sec = sec;
	itval.it_value.tv_nsec = ns;
	ret = timerfd_settime(fd, 0, &itval, NULL);
	return ret;
}

static void wait_period(struct periodic_info *info)
{
	unsigned long long missed;
	int ret;

	/* Wait for the next timer event. If we have missed any the
	   number is written to "missed" */
	ret = read(info->timer_fd, &missed, sizeof(missed));
	if (ret == -1) {
		perror("read timer");
		return;
	}

	info->wakeups_missed += missed;
}

static inline int gettid(void)
{
	return syscall(SYS_gettid);
}

void set_cpuid(int cpuid)
{
	cpu_set_t set;
	CPU_ZERO(&set);
	CPU_SET(cpuid, &set);

	if (sched_setaffinity(0, sizeof(set), &set) == -1) {
		perror("sched_setaffinity");
		exit(-1);
	}
//	printf("TID %d: assigned to CPU %d\n", gettid(), cpuid);
}

int thread_1_count;
static void *thread_1(void *arg)
{
	struct periodic_info info;
	unsigned long long C = atoi(  ((char**)arg)[1] )*100*1000; //
	unsigned long long T = atoi( ((char**)arg)[2])*1000;
	unsigned int cpu =atoi( ((char**)arg)[3]);
	unsigned long long i;
	unsigned long long C_for = C + (C/10)*2;
		printf("PID: %d, C: %llu, T: %llu, CPUID: %d\n", getpid(),C,T,cpu);	
	printf("Thread 1: TID %d\n", gettid());
	set_cpuid(cpu);
	printf("TID %d: period 10ms\n", gettid());
	make_periodic(T, &info); // 10000 us
	while (1) {
		for(i = 0; i < C_for;i++);
		wait_period(&info);
	}
	return NULL;
}


int main(int argc, char* argv[]){
	
	int r;
	pid_t pid;
	struct timespec C,T;
	r = syscall(399,pid,C,T);
	r = syscall(400,pid);
	r = syscall(401,pid);	
	
	if(argc>4){
		printf("Too many arguments\n");
		return -1;
	}else if(argc<4){
		printf("Too little arguments\n");
		return -1;
	}else{

	char *execution_time = argv[1];
	char *period = argv[2];
	char *cpu_num = argv[3];
	
	// add check if correct stuff
	
	unsigned long exec_time = atoi(execution_time);
	unsigned long per_time = atoi(period);
	unsigned int cpu_number = atoi(cpu_num);
	unsigned int pid1 = getpid();
	if(cpu_number > 3 || cpu_number < 0 || exec_time > per_time || exec_time == 0 || per_time == 0 || exec_time > 10000000 || per_time > 10000000 ){
		printf("Invalid cpu core or C > T\n");
	}

	printf("PID: %d, C: %lu, T: %lu, CPUID: %d\n", pid1,exec_time,per_time,cpu_number);
	
	char* setfreq = "sudo sh disable_cpufreq.sh";
	system(setfreq);

	struct periodic_info info;
	unsigned long long C = exec_time*100*1000; //
	unsigned long long T = per_time*1000;
	unsigned int cpu = cpu_number;
	unsigned long long i;
	unsigned long long C_for = C + (C/10)*2;

	set_cpuid(cpu);

	make_periodic(T, &info); // 10000 us

	while (1) {
		for(i = 0; i < C_for;i++);
		wait_period(&info);
	}
		return 0;
    }	

//	pthread_t t_1;
	
//	pthread_create(&t_1, NULL, thread_1,argv);
//	while(1);

	
//	printf("Thread 1: %d iterations\n", thread_1_count);

return 0;
}
