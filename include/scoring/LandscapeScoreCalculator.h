#pragma once

#include <cstddef>

#include "model/PersonalBoard.h"
#include "model/StackType.h"
#include "rules/StackRule.h"

namespace harmonies
{
    namespace scoring
    {
        std::size_t LandscapeScoreCalculator(const model::PersonalBoard &board);

    } // namespace scoring
} // namespace harmonies
