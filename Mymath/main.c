#include<stdio.h>
#include"add.h"
#include"sub.h"

int main()
{
    int a= 20;
    int b= 5;
    printf("add(%d,%d)= %d\n",a,b,add(a,b));
    printf("sub(%d,%d)= %d\n",a,b,sub(a,b));
    return 0;
}
