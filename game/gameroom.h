#pragma once

#include <unordered_set>
#include <memory>
#include <mutex>

namespace serverproject::playersession 
{
    class PlayerSession;
}

namespace serverproject::gameroom
{
    class GameRoom
    {
      public:
        GameRoom() = default;

        void joinPlayer(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer);

        void leavePlayer(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer);

        void joinHost(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer);

        void leaveHost(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer);

        void loadQuiz(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer);

        void nextQuestion(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer);

        void submitAnswer(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer);

        void sendResults(const std::shared_ptr<serverproject::playersession::PlayerSession>& aPlayer);

      private:
        std::unordered_set<std::shared_ptr<serverproject::playersession::PlayerSession>> _players;
        std::shared_ptr<serverproject::playersession::PlayerSession> _host = nullptr;
        std::mutex _playersMutex;
        std::mutex _hostMutex;
    };
}