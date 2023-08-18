/* 实现异步工作器 */
#ifndef __M_LOOPER_H__
#define __M_LOOPER_H__
#include "buffer.hpp"
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <memory>
#include <atomic>

namespace Log_System
{
    using Functor = std::function<void(Buffer &)>;
    enum class AsyncType
    {
        ASYNC_SAFE,  // 安全状态，表示缓冲区满了就阻塞，避免资源耗尽
        ASYNC_UNSAFE // 不考虑资源耗尽问题，无限扩容，用于测试
    };
    class AsyncLooper
    {
    public:
        using ptr = std::shared_ptr<AsyncLooper>;
        AsyncLooper(const Functor &cb, AsyncType loop_type = AsyncType::ASYNC_SAFE)
            : _stop(false), _thread(std::thread(&AsyncLooper::ThreadEntry, this)), _call_back(cb), _looper_type(loop_type)
        {
        }
        ~AsyncLooper()
        {
            stop();
        }
        void stop()
        {
            _stop = true;           // 将退出标志设置为true
            _cond_con.notify_all(); // 唤醒所有工作线程
            _thread.join();         // 等待工作线程退出
        }
        void push(const char *data, size_t len)
        {
            if (_stop == true)
                return;
            {
                // 1. 无限扩容-非安全 2. 固定大小-生产缓冲区中数据满就阻塞
                std::unique_lock<std::mutex> lock(_mutex);
                // 条件变量空值，若缓冲区剩余大小大于数据长度，则可以添加数据
                if (_looper_type == AsyncType::ASYNC_SAFE)
                {
                    _cond_pro.wait(lock, [&]()
                                   { return _pro_buf.WriteAbleSize() >= len; });
                }
                // 走下来代表条件满足，可以添加数据
                _pro_buf.Push(data, len);
            }

            // 唤醒消费者对缓冲区中的数据进行处理
            _cond_con.notify_one();
        }

    private:
        // 线程入口函数 - 对消费缓冲区中的数据处理，处理完毕后，初始化缓冲区，交换缓冲区
        void ThreadEntry()
        {
            while (1)
            {
                // 为互斥锁设置一个生命周期，缓冲区交换完毕时就解锁（不对数据处理过程加锁保护）
                {
                    // 1.判断生产缓冲区有没有数据，有则交换，无则阻塞
                    std::unique_lock<std::mutex> lock(_mutex);
                    // 退出标志被设置，且生产缓冲区无数据，再退出，否则可能生产缓冲区有数据，但没有完全被处理
                    // 如果不是死循环，当收到退出信号而唤醒等待时，就会因为不满足循环条件而跳出循环，导致有数据没有被处理
                    if (_stop && _pro_buf.Empty())
                        break;

                    // 退出前被唤醒，或者有数据被唤醒，返回真，继续向下运行
                    _cond_con.wait(lock, [&]()
                                   { return _stop || !_pro_buf.Empty(); }); // 生产缓冲区为空就阻塞消费
                    _con_buf.Swap(_pro_buf);
                    // 2.交换缓冲区过后，生产缓冲区为空，唤醒生产者
                    if (_looper_type == AsyncType::ASYNC_SAFE)
                    {
                        _cond_pro.notify_all();
                    }
                }
                // 3. 被唤醒后，对消费缓冲区处理数据
                _call_back(_con_buf);
                // 4. 初始化消费缓冲区
                _con_buf.Reset();
            }
        }

    private:
        Functor _call_back; // 具体对缓冲区数据处理的回调函数，由异步工作器使用者传入
    private:
        std::atomic<bool> _stop; // 工作器停止标志
        Buffer _pro_buf;         // 生产缓冲区
        Buffer _con_buf;         // 消费缓冲区
        std::mutex _mutex;       // 锁
        std::condition_variable _cond_pro;
        std::condition_variable _cond_con;
        std::thread _thread;
        AsyncType _looper_type;
    };
}

#endif