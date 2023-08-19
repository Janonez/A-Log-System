#include "../logs/LogSystem.h"
/*
扩展一个以时间作为日志文件滚动切换类型的日志落地模块：
    以时间进行文件滚动，实际上是以时间段进行滚动
    实现思想：以当前系统时间%时间段大小，得到当前时间的第几个时间段，判断当前文件的时间段是否一致
*/


void test_log(const std::string &name)
{
    INFO("%s", "测试开始");

    Log_System::Logger::ptr logger = Log_System::LoggerManger::getInstance().getLogger(name);
    logger->debug("%s", "测试日志");
    logger->info("%s", "测试日志");
    logger->warn("%s", "测试日志");
    logger->error("%s", "测试日志");
    logger->fatal("%s", "测试日志");

    INFO("%s", "测试结束");
}

int main()
{
    std::unique_ptr<Log_System::LoggerBuilder> builder(new Log_System::GlobalLoggerBuilder());
    builder->buildLoggerName("sync_logger");
    builder->buildLoggerType(Log_System::LoggerType::LOGGER_SYNC);
    builder->buildLoggerLevel(Log_System::LogLevel::value::DEBUG);
    builder->buildFormatter("[%c][%f:%l]%m%n");
    builder->buildLoggerSink<Log_System::FileSink>("./logfile/sync.log");
    builder->buildLoggerSink<Log_System::StdoutSink>();
    builder->buildLoggerSink<Log_System::ROLLBySizeSink>("./logfile/roll-sync-by-size", 1024 * 1024);
    builder->build();

    test_log("sync_logger");
    return 0;
}