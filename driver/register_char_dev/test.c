#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	int rt;
	
	errno=0;
	rt=open("/dev/register_test_cdev",0777);
	printf("rt=%d\n",rt);
	printf("error=%d\n",errno);

	return 0;
}