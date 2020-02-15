#include<iostream>
#include<poll.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
int main()
{
    struct pollfd ev[1];
    ev[0].fd = 0;
    ev[0].events = POLLIN;
    ev[0].revents = 0;
    for(;;)
    {
        switch(poll(ev,1,-1))
        {
            case 0:
                std::cout << "timeout ..." << std::endl;
                break;
            case -1:
                perror("poll");
                break;
            default:
                if(ev[0].revents & POLLIN)
                {
                    char buf[1024];
                    ssize_t s = read(0,buf,sizeof(buf) - 1);
                    buf[s] = 0;
                    std::cout << buf << std::endl;
                }
                break;
        }
    }
    return 0;
}
