#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

int main()
{
    umask(0);
    int fd = open("myfile",O_WRONLY|O_CREAT,0644);
    if(fd < 0)
    {
        perror("open");
        return 1;
    }
    const char *msg = "hello world";
    write(fd,msg,strlen(msg));
    char buf[1024];
    ssize_t s = read(fd,buf,strlen(msg));
    printf("%s\n",buf);
    if(s > 0)
    {
        printf("%s",buf);
    }
    printf("%d\n",fd);
    close(fd);
    return 0;
}
