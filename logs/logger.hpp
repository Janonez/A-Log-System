/*
日志器模块：
    1. 抽象日志器基类
    2. 派生出不同的子类（同步日志器类 & 异步日志器类）
*/
#ifndef __M_LOGGER_H__
#define __M_LOGGER_H_
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <cstdarg>
#include "util.hpp"
#include "level.hpp"
#include "format.hpp"
#include "message.hpp"
#include "sink.hpp"
#include <atomic>
#include <mutex>

namespace Log_System
{
    class Logger
    {
    public:
        using ptr = std::shared_ptr<Logger>;
        Logger(const std::string &logger_name, LogLevel::value level, Formatter::ptr &formatter, std::vector<LogSink::ptr> &sinks)
            : _logger_name(logger_name), _limit_level(level), _formatter(formatter), _sinks(sinks.begin(), sinks.end())
        {
        }
        /* 完成构造日志消息对象过程并进行格式化，得到格式化后的日志消息字符串--然后进行落地输出 */
        void debug(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            // 通过传入的对象构造一个日志消息对象，进行格式化，落地输出
            // 1. 判断当前的日志是否达到了输出等级
            if (LogLevel::value::DEBUG < _limit_level)
            {
                return;
            }
            // 2. 对fmt格式化字符串，和不定参进行字符串组织，得到日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (ret == -1)
            {
                // 提取出错
                std::cout << "vasprintf failed!!" << std::endl;
                return;
            }
            va_end(ap); // 将ap指针置空
            serialize(LogLevel::value::DEBUG, file, line, res);
            free(res); // vasprintf函数会申请空间将地址赋值给res
        }
        void info(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            // 通过传入的对象构造一个日志消息对象，进行格式化，落地输出
            // 1. 判断当前的日志是否达到了输出等级
            if (LogLevel::value::INFO < _limit_level)
            {
                return;
            }
            // 2. 对fmt格式化字符串，和不定参进行字符串组织，得到日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (ret == -1)
            {
                // 提取出错
                std::cout << "vasprintf failed!!" << std::endl;
                return;
            }
            va_end(ap); // 将ap指针置空
            serialize(LogLevel::value::INFO, file, line, res);
            free(res); // vasprintf函数会申请空间将地址赋值给res
        }
        void warn(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            // 通过传入的对象构造一个日志消息对象，进行格式化，落地输出
            // 1. 判断当前的日志是否达到了输出等级
            if (LogLevel::value::WARN < _limit_level)
            {
                return;
            }
            // 2. 对fmt格式化字符串，和不定参进行字符串组织，得到日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (ret == -1)
            {
                // 提取出错
                std::cout << "vasprintf failed!!" << std::endl;
                return;
            }
            va_end(ap); // 将ap指针置空
            serialize(LogLevel::value::WARN, file, line, res);
            free(res); // vasprintf函数会申请空间将地址赋值给res
        }
        void error(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            // 通过传入的对象构造一个日志消息对象，进行格式化，落地输出
            // 1. 判断当前的日志是否达到了输出等级
            if (LogLevel::value::ERROR < _limit_level)
            {
                return;
            }
            // 2. 对fmt格式化字符串，和不定参进行字符串组织，得到日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (ret == -1)
            {
                // 提取出错
                std::cout << "vasprintf failed!!" << std::endl;
                return;
            }
            va_end(ap); // 将ap指针置空
            serialize(LogLevel::value::ERROR, file, line, res);
            free(res); // vasprintf函数会申请空间将地址赋值给res
        }
        void fatal(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            // 通过传入的对象构造一个日志消息对象，进行格式化，落地输出
            // 1. 判断当前的日志是否达到了输出等级
            if (LogLevel::value::FATAL < _limit_level)
            {
                return;
            }
            // 2. 对fmt格式化字符串，和不定参进行字符串组织，得到日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (ret == -1)
            {
                // 提取出错
                std::cout << "vasprintf failed!!" << std::endl;
                return;
            }
            va_end(ap); // 将ap指针置空
            serialize(LogLevel::value::FATAL, file, line, res);
            free(res); // vasprintf函数会申请空间将地址赋值给res
        }

    protected:
        void serialize(LogLevel::value level, const std::string &file, size_t line, char *str)
        {
            // 3. 构造LogMsg对象
            LogMsg msg(level, file, line, _logger_name, str);
            // 4. 通过格式化工具对msg格式化，得到日志字符串
            std::stringstream ss;
            _formatter->format(ss, msg);
            // 5. 进行日志落地
            log(ss.str().c_str(), ss.str().size());
        }
        /* 抽象接口完成实际的落地输出--不同的日志器会有不同的实际落地方式 */
        virtual void log(const char *data, size_t len) = 0;

    protected:
        std::mutex _mutex;
        std::string _logger_name;
        std::atomic<LogLevel::value> _limit_level; // 原子，保证线程安全
        Formatter::ptr _formatter;
        std::vector<LogSink::ptr> _sinks;
    };

    class SyncLogger : public Logger
    {
    public:
        SyncLogger(const std::string &logger_name, LogLevel::value level, Formatter::ptr &formatter, std::vector<LogSink::ptr> &sinks)
            : Logger(logger_name, level, formatter, sinks)
        {
        }

    protected:
        // 同步日志器，将日志直接通过落地模块句柄进行日志落地
        void log(const char *data, size_t len)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if (_sinks.empty())
                return;
            for (auto &sink : _sinks)
            {
                sink->log(data, len);
            }
        }
    };
}
#endif