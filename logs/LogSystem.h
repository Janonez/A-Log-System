#ifndef __M_LOGSYSTEM_H__
#define __M_LOGSYSTEM_H__
#include "logger.hpp"

namespace Log_System
{
    // 提供获取指定日志器的全局接口
    Logger::ptr getLogger(const std::string &name)
    {
        return LoggerManger::getInstance().getLogger(name);
    }   
    Logger::ptr rootLogger()
    {
        return LoggerManger::getInstance().rootLogger();
    }

    // 使用宏函数对日志器接口进行代理(代理模式)
    #define debug(fmt, ...) debug(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define info(fmt, ...) info(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define warn(fmt, ...) warn(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define error(fmt, ...) error(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
    #define fatal(fmt, ...) fatal(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

    // 提供宏函数，直接通过默认日志器进行标准输出打印
    #define DEBUG(fmt, ...) Log_System::rootLogger()->debug(fmt, ##__VA_ARGS__)
    #define INFO(fmt, ...) Log_System::rootLogger()->info(fmt, ##__VA_ARGS__)
    #define WARN(fmt, ...) Log_System::rootLogger()->warn(fmt, ##__VA_ARGS__)
    #define ERROR(fmt, ...) Log_System::rootLogger()->error(fmt, ##__VA_ARGS__)
    #define FATAL(fmt, ...) Log_System::rootLogger()->fatal(fmt, ##__VA_ARGS__)
}

#endif