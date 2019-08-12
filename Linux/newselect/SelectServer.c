#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int fdArray[sizeof(fd_set)*8];//定义一个数组
int startup(int port)
{
    //1.创建套接字
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)
    {
        perror("socket fail...\n");
        exit(2);
    }
    //2.解决TIME_WAIT时，服务器不能重启的问题，使服务器可以立即重启
    int opt=1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_addr.s_addr=htonl(INADDR_ANY);//地址为任意类型
    local.sin_port=htons(port);
    //3.绑定端口号
    if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
    {
        perror("bind fail...\n");
        exit(3);
    }
    //4.获取监听套接字
    if(listen(sock,5)<0)
    {
        perror("listen fail...\n");
        exit(4);
    }
    return sock;
}

int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        printf("Usage: %s port\n",argv[0]);
        return 1;
    }
    //1.获取监听套接字
    int listen_sock=startup(atoi(argv[1]));//端口号传入的时候是以字符串的形式传入的，需要将其转为整形
    
    //2.初始化数组
    fdArray[0]=listen_sock;
    int cap=sizeof(fdArray)/sizeof(fdArray[0]);//cap表示最多能描述的文件描述符
    printf("%d\n",cap);
    int i= 1;
    for(;i < cap;i++)
    {
        fdArray[i]= -1;//将数组的每个数值置为-1，是因为文件描述符的大小是从0开始递增的整数。
    }
    for(;;)
    {
        //3.根据数组向文件描述符集中添加文件描述符
        fd_set rfds;//定义一个只读文件描述符
        FD_ZERO(&rfds);//清空文件描述符集
        int max_fd=fdArray[0];
        int i=0;
        for(;i<cap;i++)
        {
            if(fdArray[i]>=0)
            {
                FD_SET(fdArray[i],&rfds);
                if(fdArray[i]>max_fd)
                {
                    max_fd=fdArray[i];//更新所关心的最大描述符
                }
            }
        }
        //4.设置select()的等待事件
        struct timeval timeout={5,0};

        //5.开始调用select等待所关心的文件描述符集
        switch(select(max_fd+1,&rfds,NULL,NULL,NULL))
        {
            case 0://表示词状态改变前已经超过了timeout的时间
                printf("timeout...\n");
            case -1://失败
                printf("select fail...\n");
            default://成功
                {
                    //6.根据数组中记录的所关心的文件描述符集先判断哪个文件描述符就绪
                    //如果是监听文件描述符，调用accept接收新连接
                    //如果是普通文件描述符，则调用read读取数据
                    int i= 0;
                    for(;i<cap;i++)
                    {
                        if(fdArray[i]==-1)
                        {
                            continue;
                        }
                        if(fdArray[i]==listen_sock && FD_ISSET(fdArray[i],&rfds))
                        {
                            //1.如果监听套接字上读就绪，此时提供接收连接服务
                            struct sockaddr_in client;
                            socklen_t len=sizeof(client);
                            int new_sock=accept(listen_sock,(struct sockaddr*)&client,&len);
                            if(new_sock<0)
                            {
                                perror("accept fail...\n");
                                continue;
                            }
                            //获取新的文件描述符之后，将该文件描述符添加进数组中，以供下一次关心该文件描述符
                            int i=0;
                            for(;i<cap;i++)
                            {
                                if(fdArray[i]==-1)
                                {
                                    break;//放到数组中第一个值为-1的位置
                                }
                            }
                            if(i<cap)
                            {
                                fdArray[i]=new_sock;
                            }
                            else
                            {
                                close(new_sock);
                            }
                            printf("get a new link!,[%s:%d]\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
                            continue;
                        }
                        //2.此时关心的是普通文件描述符，提供读取数据的服务
                        if(FD_ISSET(fdArray[i],&rfds))
                        {
                            char buf[1024];
                            ssize_t s=read(fdArray[i],buf,sizeof(buf)-1);
                            if(s<0)
                            {
                                printf("read fail...\n");
                                close(fdArray[i]);
                                fdArray[i]=-1;
                            }
                            else if(s==0)
                            {
                                printf("client quit...\n");
                                close(fdArray[i]);
                                fdArray[i]=-1;
                            }
                            else
                            {
                                buf[s]=0;
                                printf("client# %s\n",buf);
                            }
                        }
                    }
                }
                break;
        }
    }
    return 0;
}
