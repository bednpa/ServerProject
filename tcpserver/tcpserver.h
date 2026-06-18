#pragma once

#include <boost/asio.hpp>

#include "../game/gameroom.h"
#include "../threadpool/threadpool.h"

namespace serverproject::tcpserver
{
    class TcpServer
    {
      public:
        TcpServer(boost::asio::io_context& aIOContext, unsigned short aPort);

      private:
        void doAccept();

        boost::asio::ip::tcp::acceptor _acceptor;
        serverproject::gameroom::GameRoom _gameRoom;
        serverproject::threadpool::ThreadPool _threadPool;
    };
}