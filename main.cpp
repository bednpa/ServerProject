#include "./tcpserver/tcpserver.h"

int main()
{
    boost::asio::io_context ioContext;

    constexpr unsigned short port = 8080;
    serverproject::tcpserver::TcpServer server(ioContext, port);

    ioContext.run();

    return 0;
}
