#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct msgbuf
{
    long Tag;
    char message[256];
};


int main(int argc, char** argv){
     key_t key_id_UP,key_id_DOWN;
    int msgq_id_UP,msgq_id_DOWN,send_val,rec_val;
    char Msgbuff[256];
    long proc_id=getpid();
    proc_id=proc_id %10000;

    key_id_UP = ftok("UP", 37);              
    msgq_id_UP = msgget(key_id_UP, 0666 | IPC_CREAT); 

    if (msgq_id_UP == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    //DOWN queue
    key_id_DOWN = ftok("DOWN", 37);              
    msgq_id_DOWN = msgget(key_id_DOWN, 0666 | IPC_CREAT); 

    if (msgq_id_DOWN == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    while (1)
    {
        scanf("%s",Msgbuff);
        struct msgbuf buff;
        buff.Tag=proc_id;
        strcpy(buff.message,Msgbuff);
        send_val = msgsnd(msgq_id_UP, &buff, sizeof(buff.message), !IPC_NOWAIT);

        if (send_val == -1)
            perror("Error in send"); 

        rec_val = msgrcv(msgq_id_DOWN, &buff, sizeof(buff.message), proc_id, !IPC_NOWAIT);

        if (rec_val == -1)
            perror("Error in receive");
        else
            printf("Converted: %s\n", buff.message); 

    }  
}