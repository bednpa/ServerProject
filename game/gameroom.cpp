#include "gameroom.h"

#include "playersession.h"

namespace serverproject::gameroom
{
    void GameRoom::joinPlayer(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer)
    {
        {
            auto hostGuard = std::scoped_lock(_hostMutex);

            if(_host != nullptr && _host->getName() == aPlayer->getName())
            {
                aPlayer->send("Player name is already taken by host!\n");

                return;
            }
        }

        {
            auto playersGuard = std::scoped_lock(_playersMutex);

            for(const auto& player : _players)
            {
                if(player->getName() == aPlayer->getName())
                {
                    aPlayer->send("Player name is already taken by another player!\n");

                    return;
                }
            }

            _players.insert(aPlayer);
        }

        aPlayer->send("Welcome Player!\n");
    }

    void GameRoom::leavePlayer(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer)
    {
        {
            auto playersGuard = std::scoped_lock(_playersMutex);

            _players.erase(aPlayer);
        }

        aPlayer->send("See you soon Player!\n");
    }

    void GameRoom::joinHost(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer)
    {
        {
            auto playersGuard = std::scoped_lock(_playersMutex);

            for(const auto& player : _players)
            {
                if(player->getName() == aPlayer->getName())
                {
                    aPlayer->send("Host name is already taken by another player!\n");

                    return;
                }
            }
        }

        {
            auto hostGuard = std::scoped_lock(_hostMutex);

            if(_host != nullptr)
            {
                aPlayer->send("Another Host is already there!\n");

                return;
            }

            _host = aPlayer;
        }

        aPlayer->send("Welcome Host!\n");
    }

    void GameRoom::leaveHost(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer)
    {
        {
            auto hostGuard = std::scoped_lock(_hostMutex);

            _host = nullptr;
        }

        aPlayer->send("See you soon Host!\n");
    }

    void GameRoom::loadQuiz(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer)
    {
        aPlayer->send("TODO\n");
        _host = nullptr;
    }

    void GameRoom::nextQuestion(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer)
    {
        aPlayer->send("TODO\n");
        _host = nullptr;
    }

    void GameRoom::submitAnswer(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer)
    {
        aPlayer->send("TODO\n");
        _host = nullptr;
    }

    void GameRoom::sendResults(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer)
    {
        aPlayer->send("TODO\n");
        _host = nullptr;
    }
}