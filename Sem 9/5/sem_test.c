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

int P(int msqid, int n);

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

	P(msqid, 5);

	return 0;
}

int P(int msqid, int n)
{
	struct msgbuf msgbuf;

	int msg_len = sizeof(int);

	for (int i = 1; i <= n; i++)
	{
		if (msgrcv(msqid, (struct msbuf*) &msgbuf, msg_len, 1, 0) < 0)
		{
			printf("P: Can't receive message from queue\n");
			exit(-1);
		}
	}

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

int SendStopMessage(int msqid)
{
	struct msgbuf msgbuf;

	int msg_len = sizeof(int);

	msgbuf.type = 255;
	msgbuf.msg = 1;

	if (msgsnd(msqid, (struct msgbuf *) &msgbuf, msg_len, 0) < 0)
	{
		printf("SendStopMessage: Can\'t send message to queue\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
		exit(-1);
	}

	return 0;
}