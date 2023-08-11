/*实用工具类的实现
    1. 获取系统时间
    2. 判断文件是否存在
    3. 获取文件所在目录
    4. 创建目录
*/
#ifndef __M_UTIL__
#define __M_UTIL__

#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace Log_System
{
    namespace util
    {
        class Date
        {
        public:
            // 定义成静态成员接口，不需要实例化就可以通过类名::调用
            // 1. 获取系统时间
            static size_t GetTime()
            {
                return (size_t)time(nullptr);
            }
        };
        class File
        {
        public:
            // 2. 判断文件是否存在
            static bool Exists(const std::string &pathname)
            {
                // return (access(pathname.c_str(), F_OK) == 0);
                // 只能保证Linux下使用，移植性差
                // 使用int stat(const char *path, struct stat *buf);函数查看是否存在
                // 如果获取状态成功，说明文件存在，反之文件不存在
                struct stat st;
                if (stat(pathname.c_str(), &st) < 0)
                {
                    return false;
                }
                return true;
            }
            // 3. 获取文件所在目录
            static std::string Path(const std::string &pathname)
            {
                // ./abc/a.txt
                size_t pos = pathname.find_last_of("/\\");
                if (pos == std::string::npos)
                {
                    return ".";
                }
                // string substr (size_t pos = 0, size_t len = npos) const;
                // pos 截取位置， len 截取长度 左闭右开，要加上/就得pos+1
                return pathname.substr(0, pos + 1);
            }
            // 4. 创建目录
            static void CreateDirectory(const std::string &pathname)
            {
                // ./acb/bcd/
                size_t pos = 0, index = 0;
                while (index < pathname.size())
                {
                    // size_t find_first_of (const char* s, size_t pos = 0) const;
                    // s 要查找的字符中的任意一个，pos，从什么位置查找
                    pos = pathname.find_first_of("/\\", index);
                    if (pos == std::string::npos)
                    {
                        // 没有找到目录，说明需要创建
                        mkdir(pathname.c_str(), 0777);
                    }
                    std::string parent_dir = pathname.substr(0, pos);
                    // if (parent_dir == "." || parent_dir == "..")
                    // {
                    //     // 当前目录或上一级目录，就再往后寻找
                    //     index = pos + 1;
                    //     continue;
                    // }
                    if (Exists(parent_dir) == true)
                    {
                        // 父级目录存在，就再往后寻找
                        index = pos + 1;
                        continue;
                    }
                    // 目录不存在，开始创建
                    mkdir(parent_dir.c_str(), 0777);
                    index = pos + 1;
                }
            }
        };
    }
}
// test
// tttt
#endif