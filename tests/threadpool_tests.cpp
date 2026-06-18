#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include "../threadpool/threadpool.h"

int multiplyFunc(int aFst, int aSnd)
{
    return aFst * aSnd;
}

TEST_CASE("Multiply Test", "[multiply_test]")
{
    const auto vectorSize = 5;
    const auto threadsCnt = 3UL;

    serverproject::threadpool::ThreadPool pool(threadsCnt);

    std::vector<std::future<int>> results;
    results.reserve(vectorSize);

    std::vector<int> referenceResults;
    referenceResults.reserve(vectorSize);

    for(auto i = 0; i < vectorSize; i++)
    {
        results.emplace_back(pool.add(multiplyFunc, i, i));
        referenceResults.emplace_back(i * i);
    }

    for(auto i = 0UL; i < vectorSize; i++)
    {
        REQUIRE(referenceResults[i] == results[i].get());
    }
}