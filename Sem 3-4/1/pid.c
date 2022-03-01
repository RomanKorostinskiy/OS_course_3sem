#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t pid = getpid();
	pid_t ppid = getppid();

	printf("pid = %d\nppid = %d\n", pid, ppid);

	return 0;
}