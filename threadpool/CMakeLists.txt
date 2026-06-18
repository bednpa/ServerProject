add_library(ThreadPool)

target_sources(ThreadPool
    PRIVATE
        threadpool.cpp
    PUBLIC
        FILE_SET HEADERS
        FILES
            threadpool.tpp
            threadpool.h
)