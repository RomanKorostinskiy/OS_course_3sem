#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct msgbuf
{
	long type;
	int msg;
};

int V(int msqid, int n);

int SendStopMessage(int msqid);

int main()
{
	key_t key;
	int msqid;
	char pathname[] = "sem.c";

	key = ftok(pathname, 0);

	if ((msqid = msgget(key, 0666)) < 0)
	{
		printf("Can't get msqid\n");
		exit(-1);
	}

	V(msqid, 1);

	return 0;
}

int V(int msqid, int n)
{
	struct msgbuf msgbuf;

	int msg_len = sizeof(int);

	msgbuf.type = 1;
	msgbuf.msg = 1;

	for (int i = 1; i <= n; i++)
	{
		if (msgsnd(msqid, (struct msgbuf *) &msgbuf, msg_len, 0) < 0)
		{
			printf("V: Can\'t send message to queue\n");
			msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
			exit(-1);
		}
	}

	return 0;
}
