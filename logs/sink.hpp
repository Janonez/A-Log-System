/*
日志落地模块的实现：
    1. 抽象落地基类
    2. 根据不同落地方向派生子类
    3. 使用工厂模式进行创建与表示的分离
*/
#ifndef __M_SINK_H__
#define __M_SINK_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <cassert>
#include "util.hpp"

namespace Log_System
{
    class LogSink
    {
    public:
        // 使用别名， using类型别名 = 原类型
        using ptr = std::shared_ptr<LogSink>;
        LogSink() {}
        virtual ~LogSink() {}
        virtual void log(const char *data, size_t len) = 0;
    };
    // 落地方向：标准输出
    class StdoutSink : public LogSink
    {
    public:
        // 将日志消息写入到标准输出
        void log(const char *data, size_t len)
        {
            std::cout.write(data, len);
        }
    };
    // 落地方向：文件输出
    class FileSink : public LogSink
    {
    public:
        // 构造时传入文件名，并打开文件
        FileSink(const std::string &pathname)
            : _pathname(pathname)
        {
            // 1. 创建文件所在的目录
            Util::File::CreateDirectory(Util::File::Path(_pathname));
            // 2. 创建打开文件
            _ofs.open(_pathname, std::ios::binary | std::ios::app);

            assert(_ofs.is_open());
        }
        // 将日志消息写入到文件
        void log(const char *data, size_t len)
        {
            _ofs.write(data, len);
            assert(_ofs.good());
        }

    private:
        std::string _pathname;
        std::ofstream _ofs;
    };
    // 落地方向：滚动文件（以文件大小滚动）
    class ROLLBySizeSink : public LogSink
    {
    public:
        // 构造时传入文件名，并打开文件
        ROLLBySizeSink(const std::string &basename, size_t max_size)
            : _basename(basename), _max_size(max_size), _cur_size(0), _name_count(0)
        {
            std::string pathname = CreateFileName();
            // 1. 创建文件所在的目录
            Util::File::CreateDirectory(Util::File::Path(pathname));
            // 2. 创建打开文件
            _ofs.open(pathname, std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }
        // 将日志消息写入到文件, 进行大小判断，超过指定大小切换文件
        void log(const char *data, size_t len)
        {
            if (_cur_size >= _max_size)
            {
                _ofs.close(); // 关闭文件
                std::string pathname = CreateFileName();
                _ofs.open(pathname, std::ios::binary | std::ios::app);
                assert(_ofs.is_open());
                // 每次超过大小就清零
                _cur_size = 0;
            }
            _ofs.write(data, len);
            assert(_ofs.good());
            _cur_size += len;
        }

    private:
        std::string CreateFileName() // 进行大小判断，超过指定大小创建新文件
        {
            // 获取系统时间重命名
            time_t t = Util::Date::GetTime();
            struct tm lt;
            localtime_r(&t, &lt);
            std::stringstream filename;
            filename << _basename;
            filename << lt.tm_year + 1900 << lt.tm_mon + 1 << lt.tm_mday << lt.tm_hour << lt.tm_min << lt.tm_sec;
            filename << "-" << _name_count++;
            filename << ".log";
            return filename.str();
        }

    private:
        // 通过基础文件名+扩展文件名（时间）生成一个实际的文件名
        std::string _basename;
        size_t _name_count;
        std::ofstream _ofs;
        size_t _max_size;
        size_t _cur_size;
    };

    // 落地模块工厂
    class SinkFactory
    {
    public:
        template <class SinkType, class... Args> // 不同落地方向 所需要的参数不同，采用可变参数包传入
        static LogSink::ptr Create(Args &&...args)
        {
            return std::make_shared<SinkType>(std::forward<Args>(args)...);
        }
    };
}

#endif