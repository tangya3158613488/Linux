class Singleton
{
public:
    static Singleton* GetInstance()
    {
        return &m_instance;
    }
private:
    //构造函数私有
    Singleton()
    {}
    //C++11 防拷贝
    Singleton(Singleton const&) = delete;
    Singleton& operator = (Singleton const&) = delete;
};
Singleton Singleton::m_instance;//在程序入口之前就完成单例对象的初始化
