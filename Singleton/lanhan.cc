#include <iostream>
#include <pthread.h>

template <typename T>
//class Singleton
//{
//    static T* inst;
// public:
//    static T* GetInstance()
//    {
//        if(inst == NULL)
//        {
//            inst = new T();
//        }
//        return inst;
//    }
//};
//线程安全版本
template <typename T>
class Singleton
{
    volatile static T* inst;//需要设置volatile关键字，否则可能被编译器优化
    static std::mutex lock;
public:
    static T* GetInstance()
    {
        if(inst == NULL)//双重判定空指针，降低锁冲突的概率，提高性能
        {
            lock.lock();//使用互斥锁，保证多线程情况下也只能调用一次new
            if(inst == NULL)
            {
                inst = new T();
            }
            lock.unlock()
        }
        return inst;
    }
};
