#ifndef ENDGAMECHECKER_H
#define ENDGAMECHECKER_H

#include <vector>

#include "model/PersonalBoard.h"
#include "model/TokenBag.h"

namespace harmonies
{
    namespace rules
    {
        // Check if the game should end by verifying all players' board statuses and the central token bag state.
        bool isGameOver(const std::vector<const model::PersonalBoard *> &boards,
                        const model::TokenBag &bag);
    }
}

#endif
