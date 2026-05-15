#ifndef ENDGAMECHECKER_H
#define ENDGAMECHECKER_H

#include <vector>
#include "model/Player.h"
#include "model/TokenBag.h"

namespace harmonies
{
    namespace rules
    {
        class EndGameChecker
        {
        public:
            // Check if the game should end by verifying all players' board statuses and the central token bag state.
            static bool isGameOver(const std::vector<model::Player*>& players, 
                                 const model::TokenBag& bag);

        private:
            // Internal helper function to check if an individual player's board is full.
            static bool hasTwoOrFewerEmptySpaces(const model::PersonalBoard& board);
        };
    }
}

#endif