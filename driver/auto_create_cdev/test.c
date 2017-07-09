#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
int main()
{

	int rt=-1;
	int fd=-1;
	char buf[6]="hello";
	char rbuf[7];
	fd=open("/dev/testdriver",O_RDWR);
	printf("fd=%d\n",fd);
	if(fd<0)
	{
		printf("open failed\n");
	}

	rt=read(fd,rbuf,7);
	printf("rt=%d\n",rt );
	if(rt<0)
	{
		printf("read failed\n");
		return -1;
	}
	printf("rbuf=%s\n",rbuf);

	rt=write(fd,buf,6);
	printf("rt=%d\n",rt );
	
	if(rt<0)
	{
		printf("write failed\n");
		return -1;
	}
	return 0;
}