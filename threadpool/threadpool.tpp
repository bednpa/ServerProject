#pragma once

#include "threadpool.h"

namespace serverproject::threadpool
{
    template <typename Func, typename... Args>
    std::future<std::invoke_result_t<Func, Args...>>
    ThreadPool::add(Func&& aFunc, Args&&... aArgs)
    {
        using ResultType = std::invoke_result_t<Func, Args...>;

        auto newTask = std::make_shared<std::packaged_task<ResultType()>>(
            [func = std::forward<Func>(aFunc),
             ... args = std::forward<Args>(aArgs)] mutable
            { return func(std::move(args)...); });

        auto newTaskFuture = std::future<ResultType>(newTask->get_future());

        {
            auto tasksGuard = std::unique_lock(_tasksMutex);

            _tasks.emplace([newTask]() { (*newTask)(); });
        }

        _condition.notify_one();

        return newTaskFuture;
    }
}