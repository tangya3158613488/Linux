#include "queue.hpp"
#include <time.h>
void* consumer(void* arg)
{
    BlockQueue *bq = (BlockQueue*)arg;
    int data;
    for(;;)
    {
        bq->PopData(data);
        std::cout << "Consume data done : " << data << std::endl;
    }
}
void* producter(void* arg)
{
    BlockQueue *bq = (BlockQueue*)arg;
    srand((unsigned long)time(NULL));
    for(;;)
    {
        int data = rand() % 1024;
        bq->PushData(data);
        std::cout << "Product data done: " << std::endl;
    }
}
int main()
{
    BlockQueue bq;
    pthread_t c,p;
    pthread_create(&c,NULL,consumer,(void*)&bq);
    pthread_create(&p,NULL,producter,(void*)&bq);

    pthread_join(c,NULL);
    pthread_join(p,NULL);
    return 0;
}
