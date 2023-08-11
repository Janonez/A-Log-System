#include <iostream>
/*单例模式*/
// 一个类只能创建一个对象
/*饿汉模式：程序启动就会创建一个唯一的实例对象*/
// class Singleton
// {
// private:
//     static Singleton _ton;
// private:
//     Singleton()
//         : _data(99)
//     {
//         std::cout << "单例模式构造:)" << std::endl;
//     }
//     Singleton(const Singleton &) = delete;/*拷贝构造*/
//     Singleton& operator=(const Singleton &) = delete;
//     ~Singleton()
//     {}
// public:
//     static Singleton &GetInstance()
//     {
//         return _ton;
//     }
//     int GetData()
//     {
//         return _data;
//     }
// private:
//     int _data;
// };
// Singleton Singleton::_ton;

/*懒汉模式：第一次使用单例对象时创建实例对象*/
class Singleton
{
private:
    Singleton()
        : _data(99)
    {
        std::cout << "单例模式构造:)" << std::endl;
    }
    Singleton(const Singleton &) = delete; /*拷贝构造*/
    Singleton &operator=(const Singleton &) = delete;
    ~Singleton()
    {
    }

public:
    static Singleton &GetInstance()
    {
        static Singleton _ton;
        return _ton;
    }
    int GetData()
    {
        return _data;
    }

private:
    int _data;
};
int main()
{
    std::cout << Singleton::GetInstance().GetData() << std::endl;
    return 0;
}