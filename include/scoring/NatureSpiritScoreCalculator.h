#pragma once

#include <cstddef>

#include "model/NatureSpiritCard.h"
#include "model/PersonalBoard.h"

namespace harmonies
{
    namespace scoring
    {
        std::size_t natureSpiritScoreCalculator(const model::NatureSpiritCard &card, const model::PersonalBoard &board);

    } // namespace scoring
} // namespace harmonies
