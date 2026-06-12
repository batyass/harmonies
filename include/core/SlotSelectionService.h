#pragma once

#include <cstddef>
#include <vector>

#include "model/CentralBoard.h"
#include "model/TokenBag.h"
#include "model/TokenType.h"

namespace harmonies
{
    namespace core
    {
        std::vector<model::TokenType> takeTokensFromSlot(model::CentralBoard &board,
                                                         model::TokenBag &bag,
                                                         std::size_t slotIndex);

        void resetSoloMarket(model::CentralBoard &board, model::TokenBag &bag);
    }
}
