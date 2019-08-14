#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
    pid_t id=fork();
    if(id<0){
        perror("fork");
        return 1;
    }
    else if(id== 0){
        printf("I am child,pid: %d\n",getpid());
        sleep(10);
    }
    else{
        printf("I am parent,pid: %d\n",getpid());
        sleep(2);
        exit(0);
    }
    return 0;
}
