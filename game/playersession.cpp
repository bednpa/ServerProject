#include "playersession.h"

#include <iostream>

#include <nlohmann/json.hpp>

namespace serverproject::playersession
{
    PlayerSession::PlayerSession(serverproject::gameroom::GameRoom& aGameRoom, 
                                 serverproject::threadpool::ThreadPool& aThreadPool,
                                 boost::asio::ip::tcp::socket&& aSocket)
        : _socket(std::move(aSocket))
        , _strand(_socket.get_executor())
        , _gameRoom(aGameRoom)
        , _threadPool(aThreadPool)
    {

    }

    void PlayerSession::start()
    {
        std::cout << "Player connected.\n";

        doRead();
    }

    void PlayerSession::send(const std::string& aMessage)
    {
        auto self = shared_from_this();

        boost::asio::post(
            _strand,
            [this, self, aMessage]()
            {
                const auto writeInProgress = !_writeQueue.empty();

                _writeQueue.push_back(aMessage);

                if(!writeInProgress)
                {
                    doWrite();
                }
            }
        );
    }

    void PlayerSession::doWrite()
    {
        auto self = shared_from_this();

        boost::asio::async_write(
            _socket,
            boost::asio::buffer(_writeQueue.front()),
            boost::asio::bind_executor(
                _strand,
                [this, self](boost::system::error_code aErrorCode, [[maybe_unused]] size_t aLength)
                {
                    if(!aErrorCode)
                    {
                        _writeQueue.pop_front();

                        if(!_writeQueue.empty())
                        {
                            doWrite();
                        }
                    }
                    else
                    {
                        std::cout << "Player disconnected during sending message.\n";

                        _writeQueue.clear();
                    }
                }
            )
        );
    }

    std::string PlayerSession::getName()
    {
        return _name;
    }

    void PlayerSession::doRead()
    {
        auto self = shared_from_this();

        boost::asio::async_read_until(
            _socket,
            _streamBuffer,
            '\n',
            [this, self](boost::system::error_code aErrorCode, [[maybe_unused]] size_t aLength)
            {
                if(!aErrorCode)
                {
                    auto rawMessage = std::string();
                    auto inputStream = std::istream(&_streamBuffer);

                    std::getline(inputStream, rawMessage);

                    std::cout << "Player send: " << rawMessage << " \n";

                    _threadPool.add(
                        [this, self, rawMessage = std::move(rawMessage)]()
                        {
                            processMessage(rawMessage);
                        }
                    );

                    doRead();
                }
                else
                {
                    std::cout << "Player disconnected during reading message.\n";
                }
            }
        );
    }

    void PlayerSession::processMessage(const std::string& aRawMessage)
    {
        const auto parsedMessage = nlohmann::json::parse(aRawMessage, nullptr, false);

        if(!parsedMessage.is_discarded())
        {
            if(parsedMessage.contains("action") && parsedMessage.contains("id"))
            {
                if(parsedMessage.at("action") == "joinPlayer")
                {
                    _name = parsedMessage.at("id");
                    _gameRoom.joinPlayer(shared_from_this());
                }
                else if(parsedMessage.at("action") == "leavePlayer")
                {
                    _gameRoom.leavePlayer(shared_from_this());
                }
                else if(parsedMessage.at("action") == "joinHost")
                {
                    _name = parsedMessage.at("id");
                    _gameRoom.joinHost(shared_from_this());
                }
                else if(parsedMessage.at("action") == "leaveHost")
                {
                    _gameRoom.leaveHost(shared_from_this());
                }
                else if(parsedMessage.at("action") == "loadQuiz")
                {
                    _gameRoom.loadQuiz(shared_from_this());
                }
                else if(parsedMessage.at("action") == "nextQuestion")
                {
                    _gameRoom.nextQuestion(shared_from_this());
                }
                else if(parsedMessage.at("action") == "submitAnswer")
                {
                    _gameRoom.submitAnswer(shared_from_this());
                }
                else if(parsedMessage.at("action") == "sendResults")
                {
                    _gameRoom.sendResults(shared_from_this());
                }
                else
                {
                    std::cout << "Not known action\n";
                }
            } 
        }
        else
        {
            std::cout << "Parse error\n";
        }
    }
}