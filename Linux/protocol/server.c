#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/wait.h>
#include"proto.h"

typedef struct sockaddr sockaddr;
typedef struct Arg
{
    int fd;
}Arg;

void ProcessRequest(int sock)
{
    while(1)
    {
        Response rp={0,0};
        Request rq;
        read(sock,&rq,sizeof(rq));
        switch(rq.op)
        {
            case 1:
                rp.sum=rq.x+rq.y;
                break;
            case 2:
                rp.sum=rq.x-rq.y;
                break;
            case 3:
                rp.sum=rq.x*rq.y;
                break;
            case 4:
                if(rq.y==0)
                {
                    rp.status=-1;
                    break;
                }
                else
                {
                    rp.sum=rq.x/rq.y;
                    break;
                }
            case 5:
                if(rq.y==0)
                {
                    rp.status=-2;
                    break;
                }
                else
                {
                    rp.sum=rq.x%rq.y;
                    break;
                }
            default:
                rp.status=-3;
                break;
        }
        write(sock,&rp,sizeof(rp));
    }
}
void* service(void* ptr)
{
    Arg* arg=(Arg*)ptr;
    ProcessRequest(arg->fd);
    free(arg);
    return NULL;
}
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        printf("Usage:%s ip port\n",argv[0]);
        exit(1);
    }
    
    //创建套接字
    int listen_sock=socket(AF_INET,SOCK_STREAM,0);
    if(listen_sock<0)
    {
        perror("socket error");
        exit(2);
    }
    printf("Socket:%d\n",listen_sock);
    int opt=1;
    setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    
    //命名套接字
    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_port=htons(atoi(argv[2]));
    local.sin_addr.s_addr=htonl(INADDR_ANY);

    //绑定端口号
    if(bind(listen_sock,(sockaddr*)&local,sizeof(local))<0)
    {
        perror("bind error");
        exit(3);
    }

    //开始监听
    if(listen(listen_sock,5)<0)//这里允许5个客户端连接等待，如果收到更多的请求则忽略
    {
        perror("listen error");
        exit(4);
    }

    //接受请求
    printf("bind and listen success,wait accept...\n");

    for(;;)
    {
        struct sockaddr_in client;
        socklen_t len=sizeof(client);
        int new_sock=accept(listen_sock,(sockaddr*)&client,&len);
        if(new_sock<0)
        {
            perror("accept error");
        }
        printf("get new link![%s:%d]\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
        pthread_t id=0;
        Arg* arg=(Arg*)malloc(sizeof(Arg));
        arg->fd=new_sock;
        pthread_create(&id,NULL,service,&arg);
        pthread_detach(id);
    }
    return 0;
}
