#include "tcpServer.hpp"

using namespace std;

void Usage(string proc)
{
    cout << "Usage: " << proc << "port" << std::endl;
}
int main(int argc,char* argv[])
{
    if(argc != 2){
        Usage(argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    tcpServer* ts = new tcpServer(port);
    ts->InitServer();
    ts->Start();
    return 0;
}
