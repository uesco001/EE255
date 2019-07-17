#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <sys/time.h>
#define _GNU_SOURCE
#include <string.h>
#include <sched.h>
#include <unistd.h>
#include <time.h>

struct timespec diff(struct timespec start,struct timespec end);
int clock_gettime(clockid_t clk_id,struct timespec *tp);
int show_segment_info(int pid){
	return syscall(403,pid);
}
int show_vm_areas(int pid){
	return syscall(404,pid);
}
int main(int argc, char* argv[])
{
	
	int n= atoi(argv[1]);
	char *buf = (char*) malloc(n*sizeof(char));
	int i;

	/*Check memory access time */
	struct timespec time1 = {0,0};
	struct timespec time2 = {0,0};
	struct timespec dt = {0,0};
	int t1,t2,t3;
	unsigned int ns1,ns2,dns;

	t1 = clock_gettime(CLOCK_REALTIME, &time1);
	ns1 = time1.tv_sec*1000000000 + time1.tv_nsec;
	printf("CLOCK_REALTIME START: %dsec %dnsec\n",time1.tv_sec, time1.tv_nsec); 
	printf("%dns\n", ns1);	

	for(i=0;i<n; i += 4096)
	{
	  buf[i] = 1;  
	}
	
	t2 = clock_gettime(CLOCK_REALTIME, &time2);
	ns2 = time2.tv_sec*1000000000 + time2.tv_nsec;
	printf("CLOCK_REALTIME END: %dsec %dnsec\n",time2.tv_sec, time2.tv_nsec); 
	printf("%dns\n",ns2);

	dt = diff(time1,time2);	
	printf("CLOCK_REALTIME DIFF: %dsec %dnsec\n",dt.tv_sec, dt.tv_nsec); 
	int r = show_segment_info(getpid());	
	if(r){
		printf("wew\n");
	}else{
		printf("boo\n");
	}
	 r = show_segment_info(900);
	if(r){
		printf("wew\n");
	}else{
		printf("boo\n");
	}
	 r = show_vm_areas(getpid());
	if(r){
		printf("wew1\n");
	}else{
		printf("boo1\n");
	}
	 r = show_vm_areas(900);
	if(r){
		printf("wew1\n");
	}else{
		printf("boo1\n");
	}
	while(1);
	return 0;
}

struct timespec diff(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if((end.tv_nsec - start.tv_nsec) <0)
	{
	  temp.tv_sec = end.tv_sec - start.tv_sec -1;
	  temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	}
	else{
	  temp.tv_sec = end.tv_sec - start.tv_sec;
	  temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	return temp;
}



