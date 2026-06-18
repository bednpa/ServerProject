find_package(Boost REQUIRED CONFIG)

add_library(TcpServer)

target_sources(TcpServer
    PRIVATE
        tcpserver.cpp
    PUBLIC
        FILE_SET HEADERS
        FILES
            tcpserver.h
)

target_link_libraries(TcpServer
    PUBLIC
        Boost::headers
        Game
        ThreadPool
)