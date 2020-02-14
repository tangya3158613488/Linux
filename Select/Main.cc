#include"SelectServer.hpp"

void Usage(string proc)
{
    cout << "Usage: " << proc << " port" << endl;
}
int main(int argc,char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }
    SelectServer *ss = new SelectServer(atoi(argv[1]));
    ss->InitServer();
    ss->Start();
}
