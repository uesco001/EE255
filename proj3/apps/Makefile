CC=gcc
CPP=gpp

all:
	$(CROSS_COMPILE)$(CC) -lrt -o mem_alloc mem_alloc.c
	$(CROSS_COMPILE)$(CC) -lrt -o mem_alloc_lock mem_alloc_lock.c
clean:
	rm mem_alloc mem_alloc_lock
