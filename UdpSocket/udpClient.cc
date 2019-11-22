#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
void Usage(string proc)
{
    cerr << proc << "server_ip server_port" << endl;
}

int main(int argc,char* argv[])
{
    if(argc != 3){
        Usage(argv[0]);
        return 1;
    }
    int sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock < 0){
        cerr << "socket error" << endl;
        return 2;
    }
    cout << "sock: " << sock << endl;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);

    char buf[1024];
    for(;;){
        cout << "Please Enter# ";
        cin >> buf;
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&server,sizeof(server));
        ssize_t s = recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&peer,&len);
        if(s > 0){
            buf[s] = 0;
            cout << "server echo# " << buf << endl;
            sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&peer,len);
        }
    }
    close(sock);
    return 0;
}
