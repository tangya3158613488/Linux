#pragma once

#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<vector>
#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<sys/epoll.h>

#define MAX 128
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
    void Socket()
    {
        sock = socket(AF_INET,SOCK_STREAM,0);
        if(sock < 0)
        {
            perror("socket");
            exit(2);
        }
        int opt = 1;//服务器主动把连接关闭了，为了保证服务器能够重启
        setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    }
    void Bind(int port)
    {
       struct sockaddr_in local;
       local.sin_family = AF_INET;
       local.sin_addr.s_addr = htonl(INADDR_ANY);
       local.sin_port = htons(port);

       if(bind(sock,(sockaddr*)&local,sizeof(local)) < 0)
       {
           perror("bind");
            exit(3);
       }
    }
    void Listen()
    {
        if(listen(sock,backlog) < 0)
        {
            perror("listen error");
            exit(4);
        }
    }
    int GetSock()
    {
        return sock;
    }
    int Accept()
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int fd = accept(sock,(struct sockaddr*)&peer,&len);
        if(fd < 0)
        {
            cout << "accept error" << endl;
        }
        else
        {
            cout << "get a new client ... link done" <<endl;
        }
        return fd;
    }
    ~Sock()
    {
        if(sock >= 0)
        {
            close(sock);
        }
    }
};
int Sock::backlog = 5;
class EpollServer
{
private:
     int port;
     int epfd;
     Sock sock;
public:
     EpollServer(int _port = 8080)
         : port(_port)
         , epfd(-1)
    {}

     void InitServer()
     {
          sock.Socket();
          sock.Bind(port);
          sock.Listen();
          epfd = epoll_create(256);
          if(epfd < 0)
          {
              cout << "epoll_create error!" << endl;
              exit(5);
          }
     }
     void HandlerEvents(struct epoll_event *revs,int num,int linksock)
     {
         struct epoll_event _ev;
        for(int i = 0; i < num; i++)
        {
            int fd = revs[i].data.fd;
            uint32_t ev = revs[i].events;
            if(ev & EPOLLIN)
            {
                if(fd == linksock)//链接事件就绪
                {
                    int _sock = sock.Accept();
                    if(_sock < 0)
                    {
                        continue;
                    }
                    _ev.data.fd = _sock;
                    _ev.events = EPOLLIN;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,_sock,&_ev);
                }
                else//读事件就绪
                {
                    char buf[10240];
                    ssize_t s = recv(fd,buf,sizeof(buf) - 1,0);
                    if( s > 0)
                    {
                        cout << buf << endl;
                        _ev.data.fd = fd;
                        _ev.events = EPOLLOUT;
                        epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&_ev);
                    }
                    else if(s == 0)//对端把文件描述符关闭了
                    {
                        close(fd);
                        ev = 0;
                        epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
                        cout << "client quit" << endl;
                    }
                    else
                    {
                        close(fd);
                        ev = 0;
                        cerr << "client error" << endl;
                    }
                }
            }
            if(ev & EPOLLOUT)
            {
                 string echo_http = "HTTP/1.0 200 OK\r\n\r\n<html><head></head><body><h1>hello Epoll Server!</h1></body></html>";
                 send(fd,echo_http.c_str(),echo_http.size(),0);
                 close(fd);
                 epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
                 cerr << "echo http done" << endl;
            }
        }
     }
     void Start()
     {
         struct epoll_event ev;
         ev.events = EPOLLIN;
         ev.data.fd = sock.GetSock();
         epoll_ctl(epfd,EPOLL_CTL_ADD,sock.GetSock(),&ev);
         int timeout = 1000;
         struct epoll_event revs[MAX];//返回的已经就绪的事件
         for(;;)
         {
             int num = epoll_wait(epfd,revs,MAX,-1/*timeout*/);
             switch(num)
             {
                case -1:
                    cerr << "epoll_wait error" << endl;
                    break;
                case 0:
                    cout << "timeout ..." << endl;
                    break;
                default:
                    HandlerEvents(revs,num,sock.GetSock());
                    break;
             }
         }
     }
     ~EpollServer()
     {
         if(epfd >= 0)
         {
              close(epfd);
         }
     }
};
