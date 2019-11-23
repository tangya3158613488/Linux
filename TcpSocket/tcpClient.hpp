#pragma once

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <string>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class tcpClient
{
private:
    int sock;
    std::string ip;
    int port;
public:
    tcpClient(std::string ip_,int port_)
        :ip(ip_)
        ,port(port_)
        ,sock(-1)
    {}
    void InitClient()
    {
        sock = socket(AF_INET,SOCK_STREAM,0);//创建套接字
        if(sock < 0){
        //如果套接字创建出错，打印出错信息
            std::cerr << "socket error" << std::endl;
            exit(2);
        }
    }
    void Start()
    {
       struct sockaddr_in peer;//该结构体用以存储与服务器建立连接的数据
       bzero(&peer,sizeof(peer));//将结构体清零
       peer.sin_family = AF_INET;
       peer.sin_port = htons(port);//使用htons将本地端口号转换为网络字节序的端口号
       peer.sin_addr.s_addr = inet_addr(ip.c_str());//将IP地址转换成字符串形式跨网络传输
       //与服务器建立连接
       if(connect(sock,(struct sockaddr*)&peer,sizeof(peer)) < 0){
           std::cerr << "connect error" << std::endl;
           exit(3);
       }
       //建立成功后，客户端向服务器发送请求
       char buf[1024];
       std::string in;
       while(1){
        std::cout << "Please Enter# ";
        std::cin >> in;
        write(sock,in.c_str(),in.size());
        ssize_t s = read(sock,buf,sizeof(buf));
        if(s > 0){
            buf[s] = 0;
            std::cout << "server echo# " << buf << std::endl;
        }else if(s == 0){
            std::cout << "server don't have meaasge" << std::endl;
        }else{
            std::cout << "read error" << std::endl;
            exit(4);
        }
      }
    }
    ~tcpClient()
    {
        if(sock > 0)
            close(sock);
    }
};
