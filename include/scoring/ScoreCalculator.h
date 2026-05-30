#pragma once

#include "model/Player.h"
#include "model/ScoreReport.h"

namespace harmonies
{
    namespace scoring
    {
        model::ScoreReport calculateTotalScore(const model::Player& player);
    }
}