#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
    pid_t pid;
    pid=fork();
    if(pid < 0){
        printf("%s fork error\n",__FUNCTION__);
        return 1;
    }else if(pid == 0){//child
        printf("child is run,pid is: %d\n",getpid());
        sleep(5);
        exit(2);
    }else{
        int status= 0;
        pid_t ret=waitpid(-1,&status,0);
        if(WIFEXITED(status)&& ret== pid){
            printf("wait child is successful,child return code is: %d.\n",WEXITSTATUS(status));
        }else{
            printf("wait child failed,return.\n");
            return 1;
        }
    }
    return 0;
}
