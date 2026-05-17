#pragma once

#include <cstddef>

#include "model/AnimalCard.h"

namespace harmonies
{
    namespace scoring
    {
        std::size_t AnimalCardScoreCalculator(const model::AnimalCard &card);

    } // namespace scoring
} // namespace harmonies
