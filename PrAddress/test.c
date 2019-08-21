#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

//int g_val= 0;
//int main()
//{
//    pid_t id=fork();
//    if(id< 0){
//        perror("fork");
//        return 1;
//    }
//    else if(id== 0){
//        printf("child[%d]: %d : %p\n",getpid(),g_val,&g_val);
//    }
//    else{
//        printf("parent[%d]: %d: %p\n",getpid(),g_val,&g_val);
//    }
//    sleep(2);
//    return 0;
//}
int g_val= 0;
int main()
{
    pid_t id=fork();
    if(id< 0){
        perror("fork");
        return 1;
    }
    else if(id== 0){
        g_val= 100;
        printf("child[%d]: %d : %p\n",getpid(),g_val,&g_val);
    }
    else{
        sleep(3);
        printf("parent[%d]: %d: %p\n",getpid(),g_val,&g_val);
    }
    sleep(1);
    return 0;
}
