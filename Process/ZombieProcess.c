#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
    pid_t id=fork();
    if(id < 0){
        perror("fork");
        return 1;
    }
    else if(id > 0){
        printf("parent[%d] is sleeping...\n",getpid());
        sleep(20);
    }
    else{
        printf("child[%d] is begin Z...\n",getpid());
        sleep(5);
        exit(EXIT_SUCCESS);
    }
    return 0;
}
