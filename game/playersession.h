#pragma once

#include <memory>

#include <boost/asio.hpp>

#include "../threadpool/threadpool.h"
#include "gameroom.h"

namespace serverproject::playersession
{
    class PlayerSession : public std::enable_shared_from_this<PlayerSession>
    {
      public:
        PlayerSession(serverproject::gameroom::GameRoom& aGameRoom, 
                      serverproject::threadpool::ThreadPool& aThreadPool,
                      boost::asio::ip::tcp::socket&& aSocket);

        void start();

        void send(const std::string& aMessage);

        std::string getName();

      private:
        void doRead();

        void doWrite();

        void processMessage(const std::string& aRawMessage);

        boost::asio::streambuf _streamBuffer;
        boost::asio::ip::tcp::socket _socket;
        boost::asio::strand<boost::asio::ip::tcp::socket::executor_type> _strand;
        std::deque<std::string> _writeQueue;
        serverproject::gameroom::GameRoom& _gameRoom;
        serverproject::threadpool::ThreadPool& _threadPool;
        std::string _name;
    };
}