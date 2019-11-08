#include "circlequeue.hpp"
#include <unistd.h>

void* producter(void* arg)
{
    RingQueue *rq = (RingQueue*)arg;
    srand((unsigned long)time(NULL));
    for(;;)
    {
        int data = rand() % 1024;
        rq->PutData(data);
        std::cout << "Product data done: " << data << std::endl;
        sleep(1);
    }
}
void* consumer(void* arg)
{
    RingQueue *rq = (RingQueue*)arg;
    int data;
    for(;;)
    {
        rq->GetData(data);
        std::cout << "Consume data done : " << data << std::endl;
        sleep(1);
    }
}
int main()
{
    RingQueue rq;
    pthread_t c,p;
    pthread_create(&c,NULL,consumer,(void*)&rq);
    pthread_create(&p,NULL,producter,(void*)&rq);

    pthread_join(c,NULL);
    pthread_join(p,NULL);
}
