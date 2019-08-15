#include<stdio.h>
#include<stdlib.h>

//int main()
//{
//    printf("hello PATH!\n");
//    return 0;
//}
//
int main()
{
    char* env=getenv("MYENV");
    if(env){
        printf("%s\n",env);
    }
    return 0;
}
