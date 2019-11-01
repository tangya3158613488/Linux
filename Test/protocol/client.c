#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<error.h>
#include<string.h>
#include<sys/types.h>
#include<stdlib.h>
#include"proto.h"

typedef struct sockaddr sockaddr;

int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        printf("Usage:%s ip port\n",argv[0]);
        return 1;
    }
    //创建套接字
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
        perror("socket");
        return 2;
    }
    printf("Socket:%d\n",sock);
    struct sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_port=htons(atoi(argv[2]));
    server.sin_addr.s_addr=inet_addr(argv[1]);
    
    //建立连接
    if(connect(sock,(sockaddr*)&server,sizeof(server))<0)
    {
        perror("connect");
        return 3;
    }

    char buf[128];
    Request rq;
    Response rp;
    while(1)
    {
        printf("Please Enter<x,y>:");
        scanf("%d%d",&rq.x,&rq.y);
        fflush(stdout);
        printf("Please Enter op[1(+),2(-),3(*),4(/),5(%)]:");
        scanf("%d",&rq.op);

        write(sock,&rq,sizeof(rq));
        read(sock,&rp,sizeof(rp));
        printf("status:%d,result:%d\n",rp.status,rp.sum);
    }
    return 0;
}
