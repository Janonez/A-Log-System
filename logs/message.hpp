/*
定义日志类消息，进行日志中间信息的存储
    1. 日志的输出时间（过滤日志输出时间）
    2. 日志等级（进行日志过滤分析）
    3. 源文件名称
    4. 源代码行号（定位出现错误的代码位置）
    5. 线程ID（过滤出错的线程）
    6. 日志主体消息
    7. 日志器名称（支持多日志器的同时使用）
*/

#ifndef __M_MSG_H__
#define __M_MSG_H__

#include <iostream>
#include <string>
#include <thread>
#include "level.hpp"
#include "util.hpp"

namespace Log_System
{
    struct LogMsg
    {
        LogMsg(LogLevel::value level, std::string file, size_t line, std::string logger, std::string payload)
            : _ctime(Util::Date::GetTime())
            , _level(level)
            , _file(file)
            , _line(line)
            , _tid(std::this_thread::get_id())
            , _logger(logger)
            , _payload(payload)
        {
        }

        time_t _ctime;          // 日志产生的时间戳
        LogLevel::value _level; // 日志等级
        std::string _file;      // 文件名
        size_t _line;           // 行号
        std::thread::id _tid;   // 线程ID
        std::string _logger;    // 日志器名称
        std::string _payload;   // 有效载荷
    };
};

#endif