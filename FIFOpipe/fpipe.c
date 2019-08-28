#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int argc,char *argv[])
{
    mkfifo("p",0644);
    return 0;
}
