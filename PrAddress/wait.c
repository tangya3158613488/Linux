#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<error.h>

int main()
{
    pid_t pid;
    if(pid=fork()== -1){
        perror("fork");
        exit(1);
    }
    if(pid== 0){
        sleep(5);
        exit(10);
    }
    else{
        int status;
        int ret=wait(&status);
        if(ret > 0 && (status & 0x7F)== 0){//正常退出
            printf("child exit code: %d\n",(status>>8)&0xFF);
        }
        else if(ret>0){//异常退出
            printf("sig code: %d\n",status&0x7F);
        }
    }
    return 0;

}
