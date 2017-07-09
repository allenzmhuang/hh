#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
	int rt=open("/dev/semaphore_device0",O_RDWR);
	printf("rt=%d\n",rt);
	while(1);
	return 0;
}