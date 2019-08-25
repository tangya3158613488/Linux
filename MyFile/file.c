#include<stdio.h>
#include<string.h>
#include<unistd.h>

int main()
{
    const char *s1="hello printf\n";
    const char *s2="hello fwrite\n";
    const char *s3="hello write\n";

    printf("%s",s1);
    fwrite(s2,strlen(s1),1,stdout);
    write(1,s3,strlen(s3));
    
    fork();
    return 0;
}
