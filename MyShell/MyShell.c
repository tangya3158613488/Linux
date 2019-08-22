#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

#define MAX_COM 1024
#define NUM 32

int main()
{
    char buff[MAX_COM];
    static char *argv[NUM];
    while(1)
    {
        printf("[ty@localhost minshell]$ ");
        fflush(stdout);
        fgets(buff,MAX_COM,stdin);
        buff[strlen(buff)-1]= 0;
        //解析字符串
        int argc= 0;
        argv[argc]=strtok(buff," ");//拆分字符串
        argc++;
        while(1)
        {
            char *ptr=strtok(NULL," ");
            if(ptr == NULL)
            {
                argv[argc]=NULL;
                break;
            }
            argv[argc]=ptr;
            argc++;
        }
        //执行命令
        pid_t pid=fork();//创建子进程进行程序替换
        if(pid< 0)
        {
            ;//fork创建子进程失敗
        }
        else if(pid == 0)//child
        {
            execvp(argv[0],argv);
        }
        else//parent
        {
            waitpid(pid,NULL,0);
        }
    }
    return 0;
}
