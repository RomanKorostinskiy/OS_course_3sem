#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct mymsgbuf
{
	long mtype;

	struct msg
	{
		int pid;
		char text[50];
	} msg;
};

int const MAX_MSG_LEN = sizeof(struct msg);

int SendMessage(int msqid, struct mymsgbuf* msgbuf, int pid);

int GetResponce(int msqid, struct mymsgbuf* msgbuf, int pid);

int main()
{
	struct mymsgbuf msgbuf;
	int msqid;
   	char pathname[] = "server.c";
	key_t key;
	int pid = getpid();

	key = ftok(pathname, 0);

	if ((msqid = msgget(key, 0666)) < 0){
		printf("Can\'t get msqid\n");
		exit(-1);
	}

	SendMessage(msqid, &msgbuf, pid);

	GetResponce(msqid, &msgbuf, pid);

	return 0;
}

int SendMessage(int msqid, struct mymsgbuf* msgbuf, int pid)
{
	msgbuf->mtype = 1;
	msgbuf->msg.pid = pid;

    sprintf(msgbuf->msg.text, "This is message from pid %d to server", pid);

	if (msgsnd(msqid, (struct msgbuf *) msgbuf, MAX_MSG_LEN, 0) < 0){
		printf("SendMessage: Can\'t send message to queue\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
		exit(-1);
	}

	return 0;
}

int GetResponce(int msqid, struct mymsgbuf* msgbuf, int pid)
{
	if (msgrcv(msqid, (struct msbuf*) msgbuf, MAX_MSG_LEN, pid, 0) < 0){
		printf("GetResponce: Can't receive message from queue\n");
		exit(-1);
	}

	printf("Pid %d receive message from server: %s\n", pid, msgbuf->msg.text);

	return 0;
}