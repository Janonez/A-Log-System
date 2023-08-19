#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

// 不定参宏函数
#define LOG(fmt, ...) printf("[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__);

// 不定参函数的使用，不定参数据的访问
// 使用 ... 表示不定参
// 用va_start(),va_arg(),va_end()函数读取不定参数，类似指针
void PrintfNum(int count, ...)
{
    va_list ap;
    va_start(ap,count);// 获取指定参数的起始地址，count参数下一个参数的起始地址
    for(int i = 0; i < count; ++i)
    {
        int num = va_arg(ap, int);//提取数据
        printf("param[%d]: %d\n",i+1, num);
    }
    va_end(ap);// 将ap指针置空
}

void myprintf(const char* fmt, ...)
{
    va_list ap;
    va_start(ap,fmt);
    char* res;
    int ret = vasprintf(&res, fmt, ap);
    if(ret != -1)
    {
        printf(res);
        free(res);
    }
    va_end(ap);// 将ap指针置空
}
int main()
{
    // LOG("%s-%s\n","hello", "world");// 两个参数
    // LOG("hello world\n");// 在__VA_ARGS__前加##就可以不传参数
    myprintf("%s-%d\n", "hello", 666);
    return 0;
}