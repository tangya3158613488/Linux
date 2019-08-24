#include<stdio.h>
#include<string.h>

int main()
{
    FILE *fp=fopen("myfile","r");
    if(!fp)
    {
        printf("fopen error\n");
    }
    char buf[1024];
    const char *msg="hello fread!\n";
    while(1)
    {
        ssize_t s=fread(buf,1,strlen(msg),fp);
        if(s>0)
        {
            buf[s]=0;
            printf("%s",buf);
        }
        if(feof(fp))//检测流上的文件结束符，如果文件结束，则返回非0值，否则返回0
        {
            break;
        }
    }
    fclose(fp);
    return 0;
}
