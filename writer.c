#include <stdint.h>
#include<unistd.h> 	
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<time.h>

#define BUF_LEN 4
#define TIMES 300 

static char *dev_name = "/dev/dummydrv";

static uint64_t time_in_ns()
{
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	return (uint64_t) tp.tv_sec * (1000ULL * 1000ULL * 1000ULL) +
	                  tp.tv_nsec;
}

int main(int argc,char *argv[])
{
	int fd;
	int i;
	int nwrite;
	uint64_t start, end, use;

	fd = open(dev_name, O_RDWR);
	if (fd == -1) {
		printf("open failed.\n");
		return EXIT_FAILURE;
	}

	char *buf = (char *) malloc(BUF_LEN);
	if (NULL == buf) {
		printf("malloc error\n");
		return EXIT_FAILURE;
	}

	start = time_in_ns();

	for (i=0; i < TIMES; i++) {
		nwrite = write(fd, (const void *) buf, BUF_LEN);
		if (nwrite != BUF_LEN) {	
			printf ("write buf to driver failed,return val:%d \n", nwrite);
			return nwrite;
		}
	}

	end = time_in_ns();

	use = end - start;
	printf("each time write to kernel takes time:%3llu.%09llus\n", 
	       (unsigned long long) use / 1000000000, 
	       (unsigned long long) use % 1000000000);

	close(fd);

	return EXIT_SUCCESS;
}
