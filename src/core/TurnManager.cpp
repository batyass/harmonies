#include "core/TurnManager.h"

namespace harmonies
{
    namespace core
    {

        TurnManager::TurnManager(const std::vector<model::Player *> &gamePlayers)
            : players(gamePlayers), currentPlayerIndex(0), currentTurn(1)
        {
            if (players.empty())
            {
                throw std::invalid_argument("TurnManager: player list cannot be empty");
            }
            for (model::Player *player : players)
            {
                if (player == nullptr)
                {
                    throw std::invalid_argument("TurnManager: player list cannot contain nullptr");
                }
            }
        }

        model::Player *TurnManager::getCurrentPlayer() const
        {
            return players[currentPlayerIndex];
        }

        void TurnManager::nextTurn()
        {
            currentPlayerIndex++;

            if (currentPlayerIndex >= players.size())
            {
                currentPlayerIndex = 0;
                currentTurn++;
            }
        }

        std::size_t TurnManager::getTurnCount() const
        {
            return currentTurn;
        }

        bool TurnManager::isNewTurn() const
        {
            return currentPlayerIndex == 0;
        }

    }
}
