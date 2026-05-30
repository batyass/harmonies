#pragma once

#include <cstddef>
#include "model/BoardSide.h"

namespace harmonies
{
    namespace scoring
    {
        std::size_t evaluateSoloScore(std::size_t totalScore, model::BoardSide side, bool hasSpirit);
    }
}