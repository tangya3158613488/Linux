#include<stdio.h>
#include<unistd.h>

int main()
{
    printf("pid is : %d\n",getpid());
    while(1);
    return 0;
}
