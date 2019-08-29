#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main()
{
    int pipefd[2]={0};
    pipe(pipefd);

    pid_t id=fork();

    if(id== 0)
    {
        close(pipefd[1]);
        char buf[1024];
        while(1)
        { 
            ssize_t s=read(pipefd[0],buf,sizeof(buf)-1);
            if(s>0)
            {
                 buf[s]=0;
                 printf("I am child,I got parent's message: '%s'\n",buf);
            }
        }
    }
    else
    {
        close(pipefd[0]);
        char msg[]="hello world!";
        while(1)
        {
            write(pipefd[1],msg,strlen(msg));
            sleep(1);
        }
    }
    return 0;
}
