// C++不定参函数使用
#include <iostream>

// 特化 无参数情况
void xprintf()
{
    std::cout << std::endl;
}

template <class T, class... Args>
void xprintf(const T &v, Args &&...args)
{
    std::cout << v;
    if ((sizeof...(args)) > 0)
    {
        xprintf(std::forward<Args>(args)...);
    }
    else
    {
        xprintf();
    }
}

int main()
{
    xprintf("hello");
    xprintf("hello", "world");
    xprintf("hello", "world", 666);
    return 0;
}