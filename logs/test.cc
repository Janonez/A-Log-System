#include "util.hpp"
#include "level.hpp"
#include "format.hpp"
int main()
{
    // std::cout << Log_System::Util::Date::GetTime() << std::endl;
    // std::string pathname = "./abc/bcd/a.txt";
    // Log_System::Util::File::CreateDirectory(Log_System::Util::File::Path(pathname));

    // std::cout << Log_System::LogLevel::toString(Log_System::LogLevel::value::DEBUG) << std::endl;
    // std::cout << Log_System::LogLevel::toString(Log_System::LogLevel::value::INFO) << std::endl;
    // std::cout << Log_System::LogLevel::toString(Log_System::LogLevel::value::WARN) << std::endl;
    // std::cout << Log_System::LogLevel::toString(Log_System::LogLevel::value::ERROR) << std::endl;
    // std::cout << Log_System::LogLevel::toString(Log_System::LogLevel::value::FATAL) << std::endl;
    // std::cout << Log_System::LogLevel::toString(Log_System::LogLevel::value::OFF) << std::endl;

    Log_System::LogMsg msg(Log_System::LogLevel::value::DEBUG, __FILE__, __LINE__, "root", "格式化字符串功能测试...");
    Log_System::Formatter fmt("abc%%abc[%d{%H:%M:%S}] %m%n");
    std::string str = fmt.format(msg);
    std::cout << str << std::endl;
    return 0;
}