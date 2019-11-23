#include "tcpClient.hpp"

using namespace std;

void Usage(string proc)
{
    cout << "Usage: " << proc << "server_ip server_port"<< endl;
}

int main(int argc,char* argv[])
{
    if(argc != 3){
        Usage(argv[0]);
        exit(1);
    }
    int port = atoi(argv[2]);
    string ip = argv[1];
    tcpClient* tc = new tcpClient(ip,port);
    tc->InitClient();
    tc->Start();
    return 0;
}
