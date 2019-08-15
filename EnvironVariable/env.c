#include<stdio.h>
#include<stdlib.h>

//int main(int argc,char* argv[],char* env[])
//{
//    int i=0;
//    for(;env[i];++i){
//        printf("%s\n",env[i]);
//    }
//    return 0;
//}
//
//int main(int argc,char* argv[])
//{
//    extern char **environ;
//    int i=0;
//    for(;environ[i];i++){
//        printf("%s\n",environ[i]);
//    }
//    return 0;
//}
//
//int main()
//{
//    char *p;
//    if((p = getenv("USER")))
//        printf("USER =%s\n", p);
//        putenv("USER=test");
//        printf("USER=%s\n", getenv("USER"));
//        return 0;
//}

int main()
{
    printf("%s\n",getenv("PATH"));
    return 0;
}

