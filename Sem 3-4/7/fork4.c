#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	pid_t pid, ppid;
	int a = 0;

	int process = fork();

	pid = getpid();
	ppid = getppid();

	switch(process)
	{
	case -1:
		printf("Error\n");
		break;
	case 0:
		execl("./hello.o", "./hello.o", NULL);
		break;
	default:
		printf("Parents process\n");
		a--;
		break;
	}

	printf("My pid = %d, my ppid = %d, result = %d\n",
	(int)pid, (int)ppid, a);

	return 0;
}