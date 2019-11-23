#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class tcpServer
{
private:
    int listen_sock;
    int port;
public:
    tcpServer(int port_)
        :port(port_)
        ,listen_sock(-1)
    {}
    void InitServer()
    {
        listen_sock = socket(AF_INET,SOCK_STREAM,0);
        if(listen_sock < 0){
            std::cerr << "socket error" << std::endl;
            exit(2);
        }
        //使用setsockopt函数允许创建端口号相同但IP地址不同的多个socket描述符
        int opt = 1;
        setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
        
        struct sockaddr_in local;
        bzero(&local,sizeof(local));//结构体清零
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        //local.sin_addr.s_addr = inet_addr(ip.c_str());
        local.sin_addr.s_addr = htonl(INADDR_ANY);//不用指定IP地址，只要是给服务器指定的端口号发送的连接，服务器都能接收
        if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local)) < 0){
            std::cerr << "bind error" << std::endl;
            exit(3);
        }
        if(listen(listen_sock,5) < 0){//将套接字设置为监听状态
            std::cerr << "listen error" << std::endl;
            exit(4);
        }
        signal(SIGCHLD,SIG_IGN);
    }
    void Service(int sock,std::string ip_,int port_)
    {
        while(1){
            char buf[1024];
            ssize_t s = read(sock,buf,sizeof(buf)-1);//从sock文件描述符中读出数据放到buf所指的内存中
            if(s > 0){
                buf[s] = 0;//将最后一个字符设置为0
                std::cout << "[" << ip_ << ":" << port_ << "]" << buf << std::endl;
                write(sock,buf,strlen(buf));//将buf中的数据写进sock文件描述符中
            }else if(s == 0){//读到文件结尾，说明客户端退出了，不仅没有往套接字中写数据，还将套接字关闭了
                std::cout << "client quit!" << std::endl;
                break;
            }else{
                std::cerr << "read error" << std::endl;
                break;
            }
        }
        close(sock);
    }
    void Start()
    {
        struct sockaddr_in peer;//存储建立连接的相关数据
        socklen_t len;
        for(;;){
            len = sizeof(peer);
            int sock = accept(listen_sock,(struct sockaddr*)&peer,&len);//接收建立好的连接，并开始服务
            if(sock < 0){
                std::cerr << "accept error" << std::endl;
                continue;//失败后，继续重新访问
            }
            std::cout << "get a new client" << sock << std::endl;
            //获取客户端信息
            int port_ = ntohs(peer.sin_port);
            std::string ip_ = inet_ntoa(peer.sin_addr);

            //创建多线程，让服务器可以连接多个客户端
            pid_t id = fork();
            if(id < 0){
                std::cerr << "fork error" << std::endl;
                close(sock);
                continue;
            }else if(id == 0){
            //子进程创建成功，让子进程进行服务
                close(listen_sock);
                Service(sock,ip_,port_);
                exit(0);
            }else{
            //父进程继续accept，这里主要的问题是父进程是否等子进程？是阻塞式等待还是非阻塞式等待？
            //如果父进程不等待子进程，子进程退出资源无法回收，子进程就会变成僵尸进程；
            //如果父进程等待子进程，并采用阻塞等待时，若第一批接收的连接不退出则父进程无法接收其他客户端的发起的连接。
            //如果采用非阻塞式等待，若再也没有客户端发起连接时，父进程就会阻塞等待接收连接，从而子进程又没有被父进程等待变成僵尸进程。
            //这里我们采用的解决办法就是使用SIGCHLD信号，这样子进程不会默默退出，子进程退出时会给父进程发送SIGCHLD信号。
            //父进程只要忽略该信号，不进行处理，操作系统就会自动回收子进程的资源，就不会造成僵尸进程了。
            close(sock);
            }
        }
    }
    ~tcpServer()
    {
        if(listen_sock > 0){
            close(listen_sock);
        }
    }
};
