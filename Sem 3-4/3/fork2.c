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

	if(process == -1)
	{
		printf("Error\n");
	}
	else if(process == 0)
	{
		printf("Child process\n");
		a++;
	}
	else
	{
		printf("Parents process\n");
		a--;
	}

	printf("My pid = %d, my ppid = %d, result = %d\n",
	(int)pid, (int)ppid, a);

	return 0;
}