#include "scoring/SoloScoreEvaluator.h"

namespace harmonies
{
    namespace scoring
    {
        std::size_t evaluateSoloScore(std::size_t totalScore, model::BoardSide side, bool hasSpirit)
        {
            std::size_t suns = 0;

            // 1. Base Score Part
            if (totalScore >= 160)      suns += 8;
            else if (totalScore >= 150) suns += 7;
            else if (totalScore >= 140) suns += 6;
            else if (totalScore >= 130) suns += 5;
            else if (totalScore >= 110) suns += 4;
            else if (totalScore >= 90)  suns += 3;
            else if (totalScore >= 70)  suns += 2;
            else if (totalScore >= 40)  suns += 1;
            else                        suns += 0;

            // 2. Board Face Modifier
            if (side == model::BoardSide::A)
            {
                suns += 1;
            }
            else // model::BoardSide::B
            {
                suns += 0;
            }

            // 3. Nature's Spirit Modifier
            // Since hasSpirit is a boolean, true represents playing with a spirit card (+1 sun).
            // false represents playing without a spirit card (+2 suns) as a handicap balancing.
            if (!hasSpirit)
            {
                suns += 2;
            }
            else
            {
                suns += 1;
            }

            return suns;
        }
    }
}