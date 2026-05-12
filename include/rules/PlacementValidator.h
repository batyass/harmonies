#pragma once

#include "model/PersonalBoard.h"
#include "model/TokenType.h"
#include "utils/hexCoord.h"

namespace harmonies
{
    namespace rules
    {

        bool placementValidator(model::TokenType token, const utils::HexCoord &coord, const model::PersonalBoard &board);

    } // namespace rules
} // namespace harmonies
