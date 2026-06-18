#include "tcpserver.h"

#include <iostream>

#include "../game/playersession.h"

namespace serverproject::tcpserver
{
    TcpServer::TcpServer(boost::asio::io_context& aIOContext, unsigned short aPort)
        : _acceptor(aIOContext)
        , _threadPool(std::thread::hardware_concurrency())
    {
        auto endPoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), aPort);

        _acceptor.open(endPoint.protocol());
        _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        _acceptor.bind(endPoint);
        _acceptor.listen();

        doAccept();
    }

    void TcpServer::doAccept()
    {
        _acceptor.async_accept(
            [this](boost::system::error_code aErrorCode, boost::asio::ip::tcp::socket aSocket)
            {
                if(!aErrorCode)
                {
                    std::make_shared<serverproject::playersession::PlayerSession>(_gameRoom, 
                                                                                  _threadPool, 
                                                                                  std::move(aSocket))->start();
                }
                else
                {
                    std::cout << "Unable to start a player session!\n";
                }

                doAccept();
            }
        );
    }
}