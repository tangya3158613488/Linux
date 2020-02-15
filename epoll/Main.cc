#include"EpollServer.hpp"

int main(int argc,char* argv[])
{
    EpollServer *es = new EpollServer();
    es->InitServer();
    es->Start();
}
