#pragma once

#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/select.h>
#include<vector>
#include<stdlib.h>
#include<stdio.h>
#include<strings.h>

#define DEFAULT -1
#define QUEUE_SIZE 5
using namespace std;

class Sock
{
private:
    int sock;
    static int backlog;
public:
    Sock()
    {
        sock = -1;
    }
    int Socket()
    {
        int sock = socket(AF_INET,SOCK_STREAM,0);
        if(sock < 0)
        {
            perror("socket");
            exit(2);
        }
        int opt = 1;//服务器主动把连接关闭了，为了保证服务器能够重启
        setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
        return sock;
    }
    void Bind(int sock,int port)
    {
       struct sockaddr_in local;
       bzero(&local,sizeof(local));
       local.sin_family = AF_INET;
       local.sin_addr.s_addr = htonl(INADDR_ANY);
       local.sin_port = htons(port);

       if(bind(sock,(sockaddr*)&local,sizeof(local)) < 0)
       {
           perror("bind");
            exit(3);
       }
    }
    void Listen(int sock)
    {
        if(listen(sock,backlog) < 0)
        {
            exit(4);
        }
    }
    int Accept(int listen_sock)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int sock = accept(listen_sock,(sockaddr*)&peer,&len);
        if(sock < 0)
        {
            return -1;
        }
        return sock;
    }
    ~Sock()
    {
        if(sock >= 0)
        {
            close(sock);
        }
    }
};
int Sock::backlog = QUEUE_SIZE;
class SelectServer
{
private:
    int port;
    Sock sock;
    int listen_sock;
   static vector<int> rfdv;//读文件描述符数组
    static int cap;//select能够关注的最大文件描述符的数量
public:
    SelectServer(int _port = 8080)
        :port(_port)
        ,listen_sock(DEFAULT)
    {}
    void InitServer()
    {
        listen_sock = sock.Socket();
        sock.Bind(listen_sock,port);
        sock.Listen(listen_sock);
    }
    void Start()
    {
        rfdv[0] = listen_sock;
        for(;;)
        {
            fd_set rfds;//读文件描述符集合
            FD_ZERO(&rfds);//将读文件描述符清空
            int max_fd = listen_sock;//记录最大的文件描述符值
            rfdv[0] = listen_sock;
            for(int i = 0; i < cap; i++)
            {
                if(rfdv[i] == DEFAULT)
                {
                    continue;//表示这个位置没有有效的文件描述符
                }
                if(max_fd < rfdv[i])
                {
                    max_fd = rfdv[i];//更新最大描述符值
                }
                FD_SET(rfdv[i],&rfds);//设置描述词组rfds中相关fd的位
            }
            struct timeval timeout = {5,0};
            switch(select(max_fd+1,&rfds,nullptr,nullptr,&timeout))
            {
                case 0:
                    cout << "timeout ..." << endl;
                    break;
                case -1:
                    cout << "select error" << endl;
                    break;
                default:
                    cout << "handler start ..." << endl;
                    Handler(&rfds);//已经有文件描述符读事件就绪并处理该事件
                    cout << "handler end ..." << endl;
                    break;
            }
        }
    }
    void Handler(fd_set* rfds)
    {
        for(int i = 0; i < cap; i++)
        {
            if(rfdv[i] == DEFAULT)
            {
                continue;
            }
            if(0 == i && FD_ISSET(rfdv[i],rfds))
            {
                int fd = sock.Accept(listen_sock);
                int j;
                for(j = 1; j < cap; j++)
                {
                    if(rfdv[j] == DEFAULT)
                    {
                        cout << "get a new connect ..." << endl;
                        rfdv[j] = fd;
                        break;
                    }
                    if(j == cap)
                    {
                        cout << "rfdv is full ..." << endl;
                        close(fd);
                        break;
                    }
                }
            }
            else if(i != 0 && FD_ISSET(rfdv[i],rfds))
            {
                char buf[1024];
                ssize_t s = read(rfdv[i],buf,sizeof(buf) - 1);
                if(s > 0)
                {
                    fflush(stdout);
                    cout << buf << endl;
                }
                else if(s == 0)
                {
                    cout << "client quit ..." << endl;
                    close(rfdv[i]);
                    rfdv[i] = DEFAULT;
                }
                else
                {
                    close(rfdv[i]);
                    rfdv[i] = DEFAULT;
                    cout << "read error ..." << endl;
                }
            }
        }
    }
    ~SelectServer()
    {
            
    }
};
vector<int> SelectServer::rfdv(sizeof(fd_set)*8,DEFAULT);
int SelectServer::cap = sizeof(fd_set)*8;
