#include "../logs/LogSystem.h"

// 落地方向：滚动文件（以时间滚动）
enum class TimeGap
{
    GAP_SEC,
    GAP_MIN,
    GAP_HOUR,
    GAP_DAY
};
class ROLLByTimeSink : public Log_System::LogSink
{
public:
    // 构造时传入文件名，并打开文件
    ROLLByTimeSink(const std::string &basename, TimeGap gaptype)
        : _basename(basename)
    {
        switch (gaptype)
        {
        case TimeGap::GAP_SEC:
            _gap_size = 1;
            break;
        case TimeGap::GAP_MIN:
            _gap_size = 60;
            break;
        case TimeGap::GAP_HOUR:
            _gap_size = 3600;
            break;
        case TimeGap::GAP_DAY:
            _gap_size = 3600 * 24;
            break;
        }
        // 判断创建的是第几个时间段
        // 特殊情况 任何数%1 都等于0，
        _cur_gap = _gap_size == 1 ? Log_System::Util::Date::GetTime() : Log_System::Util::Date::GetTime() % _gap_size;
        std::string filename = CreateFileName();
        // 1. 创建文件所在的目录
        Log_System::Util::File::CreateDirectory(Log_System::Util::File::Path(filename));
        // 2. 创建打开文件
        _ofs.open(filename, std::ios::binary | std::ios::app);
        assert(_ofs.is_open());
    }
    // 将日志消息写入到文件,
    void log(const char *data, size_t len)
    {
        // 判断使用的时候是第几个时间段
        time_t cur = Log_System::Util::Date::GetTime();
        time_t cur_gap = _gap_size == 1 ? Log_System::Util::Date::GetTime() : Log_System::Util::Date::GetTime() % _gap_size;
        if (cur_gap != _cur_gap)
        {
            _ofs.close(); // 关闭文件
            std::string filename = CreateFileName();
            _ofs.open(filename, std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }
        _ofs.write(data, len);
        assert(_ofs.good());
    }

private:
    std::string CreateFileName() // 进行大小判断，超过指定大小创建新文件名
    {
        // 获取系统时间重命名
        time_t t = Log_System::Util::Date::GetTime();
        struct tm lt;
        localtime_r(&t, &lt);
        std::stringstream filename;
        filename << _basename;
        filename << lt.tm_year + 1900 << lt.tm_mon + 1 << lt.tm_mday << lt.tm_hour << lt.tm_min << lt.tm_sec;
        filename << ".log";
        return filename.str();
    }

private:
    // 通过基础文件名+扩展文件名（时间）生成一个实际的文件名
    std::string _basename;
    std::ofstream _ofs;
    size_t _cur_gap;  // 当前是第几个时间段
    size_t _gap_size; // 每个时间段的大小
};

int main()
{
    std::unique_ptr<Log_System::LoggerBuilder> builder(new Log_System::GlobalLoggerBuilder());
    builder->buildLoggerName("async_logger");
    builder->buildLoggerType(Log_System::LoggerType::LOGGER_ASYNC);
    builder->buildLoggerLevel(Log_System::LogLevel::value::WARN);
    builder->buildFormatter("[%c][%f:%l]%m%n");
    builder->buildLoggerSink<ROLLByTimeSink>("./logfile/roll-async-by-time", TimeGap::GAP_SEC);
    Log_System::Logger::ptr logger = builder->build();
    size_t cur = Log_System::Util::Date::GetTime();
    while(Log_System::Util::Date::GetTime() < cur + 5)
    {
        logger->fatal("这是一条测试日志");
        usleep(1000);
    }
    return 0;
}