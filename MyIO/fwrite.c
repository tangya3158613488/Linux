#include<stdio.h>
#include<string.h>

int main()
{
    FILE *fp=fopen("myfile","w");
    if(!fp)
    {
        printf("fopen error\n");
    }
    const char *massage="hello fwrite!\n";
    int count= 3;
    while(count--)
    { 
        fwrite(massage,strlen(massage),1,fp);
    }
    fclose(fp);
    return 0;
}
