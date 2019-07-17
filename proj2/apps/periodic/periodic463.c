/* 
 * Copyright (C) 2009 Chris Simmonds (chris@2net.co.uk)
 *
 * This is a demonstration of periodic threads using the Linux timerfd
 * interface which was introduced in GNU libc 2.8 and kernel 2.6.25.
 */

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


unsigned long long temp1,temp2;
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
	temp1 = missed;
	temp2 += missed;
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
	printf("TID %d: assigned to CPU %d\n", gettid(), cpuid);
}

int thread_1_count;
int thread_2_count;
int thread_3_count;

static void *thread_1(void *arg)
{
//	struct sched_param param;
//	int pri80=80;
//	param.sched_priority = pri80;
//	if(sched_setscheduler(getpid(),SCHED_FIFO,&param )==-1 )
//	{
//	  perror("sched_setscheduler()failed");
//	  exit(1);
//	}	
	struct periodic_info info;
	unsigned long long C = atoi( ((char**)arg)[1] ) ;
	unsigned long long T = atoi( ((char**)arg)[2] ) ;
	unsigned long long temp3;
	C = C*100*1000*6/5;
	T = T*1000;
	printf("Thread 1: TID %d\n", gettid());
	set_cpuid(2);
	printf("TID %d: period 50ms\n", gettid());
	make_periodic(T, &info); // 50000 us
	while (1) {
		//temp3 = 0;
		for (temp3 =0; temp3< C; temp3++);
		//thread_1_count++;
		wait_period(&info);
	}
	return NULL;
}


static void *thread_2(void *arg)
{
//	struct sched_param param;
//	int pri79 = 79;
//	param.sched_priority = pri79;
//	if(sched_setscheduler(getpid(),SCHED_FIFO,&param )==-1 )
//	{
//	  perror("sched_setscheduler()failed");
//	  exit(1);
//	}
	struct periodic_info info;
	unsigned long long C = atoi( ((char**)arg)[1] ) ;
	unsigned long long T = atoi( ((char**)arg)[2] ) ;
	unsigned long long temp3;
	C = 20*100*1000*6/5;
	T = 80*1000;
	printf("Thread 2: TID %d\n", gettid());
	set_cpuid(2);
	printf("TID %d: period 50ms\n", gettid());
	make_periodic(T, &info); // 50000 us
	while (1) {
		//temp3 = 0;
		for (temp3 =0; temp3< C; temp3++);
		//thread_1_count++;
		wait_period(&info);
	}
	return NULL;
}


static void *thread_3(void *arg)
{
//	struct sched_param param;
//	int pri78 = 78;
//	param.sched_priority = pri78;
//	if(sched_setscheduler(getpid(),SCHED_FIFO,&param )==-1 )
//	{
//	  perror("sched_setscheduler()failed");
//	  exit(1);
//	}
	struct periodic_info info;
	unsigned long long C = atoi( ((char**)arg)[1] ) ;
	unsigned long long T = atoi( ((char**)arg)[2] ) ;
	unsigned long long temp3;
	C = 30*100*1000*6/5;
	T = 100*1000;
	printf("Thread 3: TID %d\n", gettid());
	set_cpuid(2);
	printf("TID %d: period 50ms\n", gettid());
	make_periodic(T, &info); // 50000 us
	while (1) {
		//temp3 = 0;
		for (temp3 =0; temp3< C; temp3++);
		//thread_1_count++;
		wait_period(&info);
	}
	return NULL;
}


int main(int argc, char *argv[])
{
	//char *Cin,*Tin,*cpuid;
	char *Cin = argv[1];
	char *Tin = argv[2];
	char *cpuid = argv[3];
	unsigned long CC = atoi(Cin);
	unsigned long TT = atoi(Tin);
	unsigned int cpu = atoi(cpuid);
	unsigned int pid1 = getpid();
	//int ret;
	//struct sched_param param;
	//int pri80, pri79, pri78;
	//pri80 = 80;
	//pri79 = 79;
	//pri78 = 78;

	//Set priorities for tasks
	//param.sched_priority = pri80;
	//if(sched_setscheduler(getpid(),SCHED_FIFO,&param )==-1 )
	//{
	//  perror("sched_setscheduler()failed");
	//  exit(1);
	//}

	pthread_t t_1;
	pthread_t t_2;
	pthread_t t_3;
	//int secs,nsecs,dt;
	//pthread_attr_t attr1;
	//pthread_attr_t attr2;
	//pthread_attr_t attr3;
	struct sched_param param1;
	struct sched_param param2;
	struct sched_param param3;
	param1.sched_priority = 80;
	param2.sched_priority = 79;
	param3.sched_priority = 78;
	printf("Periodic threads using timerfd\n");

	//pthread_attr_init(&attr1);
	//pthread_attr_init(&attr2);
	//pthread_attr_init(&attr3);
	pthread_create(&t_1, NULL, thread_1, argv);
	pthread_create(&t_2, NULL, thread_2, argv);
	pthread_create(&t_3, NULL, thread_3, argv);
	pthread_setschedparam(t_1,SCHED_FIFO,&param1);
	pthread_setschedparam(t_2,SCHED_FIFO,&param2);
	pthread_setschedparam(t_3,SCHED_FIFO,&param3);

	printf("PID: %d, C: %lu, T: %lu, CPUID: %d\n", pid1,CC,TT,cpu);

	sleep(10);
	printf("Thread 1: %d iterations\n", thread_1_count);
	printf("Thread 2: %d iterations\n", thread_2_count);
	printf("temp1: %llu\n",temp1);
	printf("temp2: %llu\n",temp2);
	//printf("temp3: %llu\n",temp3);

	while (1){
	//clock_gettime(CLOCK_MONOTONIC,&start );
	
	//  while(1 ){
	//  clock_gettime(CLOCK_MONOTONIC,&curr);
	//  secs = curr.tv_sec - start.tv_sec;
	//  nsecs = curr.tv_nsec - start.tv_nsec;
	//  dt = secs*1000 + nsecs/1000000;
	//  if(dt>10) {break;}
	//  }
	
	

	//sleep(10);
	}

	return 0;
}
