/*
日志等级类设计:
    1. 定义枚举类，枚举出日志等级
    2. 提供转换接口：将枚举转换为对应字符串
*/

#ifndef __M_LEVEL_H__
#define __M_LEVEL_H__
namespace Log_System
{
    class LogLevel
    {
    public:
        /*
        强类型枚举（Strongly-typed enums），使用enum class语法来声明，是C++11中的新语法，用以解决传统C++枚举类型存在的缺陷。
        1. 限制作用域
        2. 不能隐式类型转换，需要显示调用
        3. 前置声明
        */
        enum class value
        {
            UNKNOW = 0,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            OFF
        };
        // 不想定义对象且调用类内函数，加static，静态成员函数
        static const char *toString(LogLevel::value level)
        {
            switch(level)
            {
                case LogLevel::value::DEBUG : return "DEBUG";
                case LogLevel::value::INFO : return "INFO";
                case LogLevel::value::WARN : return "WARN";
                case LogLevel::value::ERROR : return "ERROR";
                case LogLevel::value::FATAL : return "FATAL";
                case LogLevel::value::OFF : return "OFF";
            }
            return "UNKNOW";
        }
    };
}

#endif