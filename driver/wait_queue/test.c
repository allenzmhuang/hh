#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
	unsigned long time=0;
	int fd=open("/dev/wq_device",O_RDWR);
	printf("fd=%d\n",fd);
	while(1)
	{
		read(fd,&time,4);
		printf("mypid=%d,time=%ld\n",(int)getpid(),time);
	}

	close(fd);
	return 0;
}