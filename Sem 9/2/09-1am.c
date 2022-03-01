#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int LAST_MESSAGE = 255;

struct mymsgbuf
{
   long mtype;
   
   struct msg
   {
      int mnum;
      char mtext[80];
   } msg;

} mybuf;

int SendInfo(int n_of_msg, struct mymsgbuf* mybuf, int msqid);

int SendLastMsg(struct mymsgbuf* mybuf, int msqid);

int main(void)
{
   int msqid;
   char pathname[] = "09-1am.c";
   key_t  key;

   /* Create or attach message queue  */

   key = ftok(pathname, 0);

   if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
      printf("Can\'t get msqid\n");
      exit(-1);
   }

   /* Send information */

   SendInfo(10, &mybuf, msqid);

   /* Send the last message */

   SendLastMsg(&mybuf, msqid);

   return 0;
}

int SendInfo(int n_of_msg, struct mymsgbuf* mybuf, int msqid)
{
   int len = 0;

   for (int i = 1; i <= n_of_msg; i++)
   {

      sprintf(mybuf->msg.mtext, "This is text message 1000 - 7*%d = %d", i, 1000 - 7 * i);

      mybuf->mtype = 1;
      mybuf->msg.mnum = i;
      len = sizeof(struct msg);

      printf("msg: %s, text len: %ld, msg size: %ld\n", mybuf->msg.mtext, strlen(mybuf->msg.mtext), sizeof(struct msg));

      if (msgsnd(msqid, (struct msgbuf *) mybuf, len, 0) < 0){
         printf("Can\'t send message to queue\n");
         msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
         exit(-1);
      }
   }

   return 0;
}

int SendLastMsg(struct mymsgbuf* mybuf, int msqid)
{
   int len     = 0;   
   mybuf->mtype = LAST_MESSAGE;

   if (msgsnd(msqid, (struct msgbuf *) mybuf, len, 0) < 0){
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
   }

   return 0;
}