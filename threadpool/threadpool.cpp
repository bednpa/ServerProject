#include "threadpool.h"

namespace serverproject::threadpool
{
    ThreadPool::ThreadPool(size_t aNumThreads)
    {
        for(size_t i = 0; i < aNumThreads; i++)
        {
            _threads.emplace_back(
                [this]
                {
                    std::function<void()> task;

                    while(true)
                    {
                        {
                            auto tasksGuard = std::unique_lock(_tasksMutex);

                            _condition.wait(
                                tasksGuard,
                                [this] { return _stop || !_tasks.empty(); });

                            if(_stop && _tasks.empty())
                            {
                                return;
                            }

                            task = std::move(_tasks.front());

                            _tasks.pop();
                        }

                        task();
                    }
                });
        }
    }

    ThreadPool::~ThreadPool()
    {
        {
            auto tasksGuard = std::unique_lock(_tasksMutex);

            _stop = true;
        }

        _condition.notify_all();

        for(auto& thread : _threads)
        {
            thread.join();
        }
    }
}