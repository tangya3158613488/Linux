#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define NUM 16

class RingQueue
{
    private:
        std::vector<int> q;
        int cap;
        sem_t data_sem;//消费者可以消费的数据
        sem_t space_sem;//生产者可以生产的位置
        int consume_step;//消费者的下标
        int product_step;//生产者的下标
    public:
        RingQueue(int _cap = NUM)
            :q(_cap)
            ,cap(_cap)
    {
        sem_init(&data_sem,0,0);
        sem_init(&space_sem,0,cap);
        consume_step = 0;
        product_step = 0;
    }
        void PutData(const int &data)
        {
            sem_wait(&space_sem);//P操作
            q[consume_step] = data;
            consume_step++;
            consume_step %= cap;
            sem_post(&data_sem);//V操作
        }
        void GetData(int &data)
        {
            sem_wait(&data_sem);
            data = q[product_step];
            product_step++;
            product_step %= cap;
            sem_post(&space_sem);
        }
        ~RingQueue()
        {
            sem_destroy(&data_sem);
            sem_destroy(&space_sem);
        }
};
