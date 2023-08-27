#include "../logs/LogSystem.h"
#include <string>
#include <vector>
#include <thread>
#include <chrono>

void bench(const std::string &logger_name, size_t thr_count, size_t msg_count, size_t msg_len)
{
    // 1. 获取日志器
    Log_System::Logger::ptr logger = Log_System::getLogger(logger_name);
    if (logger.get() == nullptr)
        return;
    std::cout << "测试日志: " << msg_count << " 条, 总大小: " << (msg_count*msg_len)/1024 << "KB\n";
    // 2. 组织指定长度的日志消息
    std::string msg(msg_len - 1, 'A'); // 少一个字节给末尾添加换行
    // 3. 创建指定数量的线程
    std::vector<std::thread> threads;
    std::vector<double> cost_array(thr_count);
    size_t msg_per_thr = msg_count / thr_count; // 每个线程要输出的日志数
    for (int i = 0; i < thr_count; ++i)
    {
        threads.emplace_back([&, i]()
                             {
        // 4. 线程函数内部开始计时
        auto start = std::chrono::high_resolution_clock::now();
        // 5. 开始循环写日志
        for(int j = 0; j < msg_per_thr; ++j)
        {
        logger->fatal("%s", msg.c_str());
        }
        // 6. 线程函数内部结束计时
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> cost = end-start;
        cost_array[i] = cost.count();
        std::cout << "\t线程[" <<i<<"]: 输出日志数量: " << msg_per_thr << ", 耗时: " << cost.count() << "s" << std::endl; });
    }
    for(int i = 0; i < thr_count; ++i)
    {
        threads[i].join();
    }
    // 7. 计算总耗时
    // 在多线程中，每个线程都会耗费时间，但是线程是并发处理的，耗时最高的才是总消耗时间
    double max_cost = cost_array[0];
    for(int i = 1; i < thr_count; ++i)
    {
        max_cost = max_cost < cost_array[i] ? cost_array[i] : max_cost;
    }
    size_t msg_per_sec = msg_count / max_cost;
    size_t size_per_sec = (msg_count * msg_len) / (max_cost * 1024);// 总数/1024 = 单位kb
    // 8. 输出打印
    std::cout << "\t总耗时: " << max_cost << " s\n";
    std::cout << "\t每秒输出日志数量: " << msg_per_sec << " 条\n";
    std::cout << "\t每秒输出日志大小: " << size_per_sec << "KB\n";
}

void sync_bench()
{
    std::unique_ptr<Log_System::LoggerBuilder> builder(new Log_System::GlobalLoggerBuilder());
    builder->buildLoggerName("sync_logger");
    builder->buildLoggerType(Log_System::LoggerType::LOGGER_SYNC);
    builder->buildFormatter("%m%n");
    builder->buildLoggerSink<Log_System::FileSink>("./logfile/sync.log");
    builder->build();
    bench("sync_logger", 1, 1000000, 100);
    // bench("sync_logger", 3, 1000000, 100);
}
void async_bench()
{
    std::unique_ptr<Log_System::LoggerBuilder> builder(new Log_System::GlobalLoggerBuilder());
    builder->buildLoggerName("async_logger");
    builder->buildLoggerType(Log_System::LoggerType::LOGGER_ASYNC);
    builder->buildEableUnSafeAsync();// 开启非安全模式--主要是为了将实际落地时间排除在外
    builder->buildFormatter("%m%n");
    builder->buildLoggerSink<Log_System::FileSink>("./logfile/async.log");
    builder->build();
    bench("async_logger", 1, 1000000, 100);
    // bench("async_logger", 3, 1000000, 100);
}

int main()
{
    sync_bench();
    async_bench();
    return 0;
}