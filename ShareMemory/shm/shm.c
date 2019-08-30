#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

int main()
{
    key_t key=ftok(".",'a');
    if(key<0)
    {
        perror("ftok");
        return -1;
    }
    int shmid=shmget(key,1024*1024,IPC_CREAT|0644);
    if(shmid<0)
    {
        perror("shget");
        return -1;
    }

    char *p=shmat(shmid,NULL,0);
    if(p==(char *)-1)
    {
        perror("shmat");
        return -1;
    }
    pid_t pid;
    if((pid=fork())<0)
    {
        perror("fork");
        return -1;
    }
    else if(pid== 0)
    {
        strcpy(p,"hello,world");
        strcpy(p,"happy");
    }
    else
    {
        char buf[100]={0};
        wait(NULL);
        printf("p= %s\n",p);
        printf("p+3 = %s\n",p+3);
    }
    if(shmdt(p)<0)
    {
        perror("shmdt");
        return -1;
    }
    if(shmctl(shmid,IPC_RMID,NULL)<0)
    {
        perror("shmctl");
        return -1;
    }
    return 0;
}
