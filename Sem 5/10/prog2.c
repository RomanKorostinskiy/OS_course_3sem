#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int fd; size_t size; char resstring[14];
	char name[]="aaa.fifo";
	umask(0);

	if((fd = open(name, O_RDONLY)) < 0)
	{
		printf("Can\'t open FIFO for reading\n");
		exit(-1);
	}

	size = read(fd, resstring, 14);

	if(size < 0)
	{
	printf("Can\'t read string\n");
	exit(-1);
	}

	printf("prog2 have read from FIFO: %s\n",resstring);
	close(fd);
	
	return 0;
}