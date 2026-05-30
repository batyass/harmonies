#pragma once

#include "utils/hexCoord.h"
#include "model/PersonalBoard.h"
#include "model/TokenType.h"

namespace harmonies
{
    namespace core
    {
        // Attempts a full token placement action:
        // validate the move against placement rules, then apply it to the board.
        bool placeTokenOnBoard(model::PersonalBoard &board, const utils::HexCoord &coord, model::TokenType token);
    }
}
