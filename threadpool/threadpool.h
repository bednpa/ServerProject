#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace serverproject::threadpool
{
    class ThreadPool
    {
      public:
        explicit ThreadPool(size_t aNumThreads);

        ThreadPool(const ThreadPool&) = delete;

        ThreadPool(ThreadPool&&) = delete;

        ThreadPool& operator=(const ThreadPool&) = delete;

        ThreadPool& operator=(ThreadPool&&) = delete;

        ~ThreadPool();

        template <typename Func, typename... Args>
        std::future<std::invoke_result_t<Func, Args...>> add(Func&& aFunc,
                                                             Args&&... aArgs);

      private:
        std::vector<std::thread> _threads;
        std::queue<std::function<void()>> _tasks;
        std::mutex _tasksMutex;
        std::condition_variable _condition;
        bool _stop{false};
    };
}

#include "threadpool.tpp"