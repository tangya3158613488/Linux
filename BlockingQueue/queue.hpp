#pragma once
#include <iostream>
#include <queue>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM 8

class BlockQueue{
    private:
        std::queue<int> q;
        int cap;//队列容量
        pthread_mutex_t lock;//互斥锁
        pthread_cond_t full;//条件变量,判断队列是否为满
        pthread_cond_t empty;//条件变量,判断队列是否为空

    private:
        void LockQueue()
        {
            pthread_mutex_lock(&lock);//给队列加锁
        }
        void UnLockQueue()
        {
            pthread_mutex_unlock(&lock);//给队列解锁
        }
        void ProductWait()
        {
            pthread_cond_wait(&full,&lock);
        }
        void ConsumerWait()
        {
            pthread_cond_wait(&empty,&lock);
        }
        void NotifyProducter()
        {
            pthread_cond_signal(&full);
        }
        void NotifyConsumer()
        {
            pthread_cond_signal(&empty);
        }
        bool IsEmpty()
        {
            return (q.size() == 0 ? true : false);
        }
        bool IsFull()
        {
            return (q.size() == cap ? true : false);
        }
    public:
        //初始化阻塞队列
        BlockQueue(int _cap = NUM)
            :cap(_cap)
        {
            pthread_mutex_init(&lock,NULL);
            pthread_cond_init(&full,NULL);
            pthread_cond_init(&empty,NULL);
        }
        //向队列中插入数据
        void PushData(const int &data)
        {
            LockQueue();
            while(IsFull())
            {
                NotifyConsumer();//通知消费者消费
                std::cout << "queue is full,notify consumer ,product stop." << std::endl;
                ProductWait();
            }
            q.push(data);
            NotifyConsumer();//一旦有数据就唤醒消费
            UnLockQueue();
        }
        //删除队列中的数据
        void PopData(int &data)
        {
            LockQueue();
            while(IsEmpty())
            {
                NotifyProducter();//通知生产者生产
                std::cout << "queue is empty,notify product,consumer stop." << std::endl;
                ConsumerWait();
            }
            data = q.front();
            q.pop();
            NotifyProducter();//一旦有空间就通知生产者生产
            UnLockQueue();
        }
        ~BlockQueue()
        {
            pthread_mutex_destroy(&lock);
            pthread_cond_destroy(&full);
            pthread_cond_destroy(&empty);
        }
};

