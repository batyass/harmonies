#ifndef TURNMANAGER_H
#define TURNMANAGER_H

#include <cstddef>
#include <stdexcept>
#include <vector>

#include "model/Player.h"

namespace harmonies
{
    namespace core
    {

        class TurnManager
        {
        private:
            // Non-owning view: Game remains responsible for player lifetime.
            std::vector<model::Player *> players;
            std::size_t currentPlayerIndex;
            std::size_t currentTurn;

        public:
            explicit TurnManager(const std::vector<model::Player *> &gamePlayers);

            model::Player *getCurrentPlayer() const;
            void nextTurn();
            std::size_t getTurnCount() const;
            bool isNewTurn() const;
        };

    }
}

#endif
