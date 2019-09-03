#include<stdio.h>
#include<unistd.h>

int main()
{
    int count= 0;
    alarm(5);
    sleep(2);
    alarm(3);
    for(;;)
    {
        ++count;
        printf("count= %d\n",count);
    }
    return 0;
}
