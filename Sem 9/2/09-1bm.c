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

int RecieveMsg (struct mymsgbuf* mybuf, int msqid);

int main(void)
{
   int msqid;
   char pathname[] = "09-1am.c";
   key_t  key;

   key = ftok(pathname, 0);

   if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
      printf("Can\'t get msqid\n");
      exit(-1);
   }

   RecieveMsg (&mybuf, msqid);

   return 0;
}

int RecieveMsg (struct mymsgbuf* mybuf, int msqid)
{
   while(1)
   {
      int maxlen = 100;

      if ((msgrcv(msqid, (struct msgbuf *) mybuf, maxlen, 0, 0)) < 0){
         printf("Can't receive message from queue\n");
         exit(-1);
      }

      printf("message type = %ld, info = %s, number: %d\n", mybuf->mtype, mybuf->msg.mtext, mybuf->msg.mnum);

      if (mybuf->mtype == LAST_MESSAGE){
         msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
         exit(0);
      }
   }

   return 0;
}
