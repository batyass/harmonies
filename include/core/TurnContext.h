#pragma once

#include <vector>
#include "model/TokenType.h"

namespace harmonies
{
    namespace core
    {
        struct TurnContext
        {
            std::vector<model::TokenType> pendingTokens;
            bool hasTakenAnimalCard = false;
        };

    }
}