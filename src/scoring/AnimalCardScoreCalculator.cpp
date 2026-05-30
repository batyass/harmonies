#include "scoring/AnimalCardScoreCalculator.h"

namespace harmonies
{
    namespace scoring
    {
        std::size_t AnimalCardScoreCalculator(const model::AnimalCard &card)
        {
            int placedCubes = card.totalSlots() - card.getCubesOnCard();
            if (placedCubes == 0)
            {
                return 0;
            }

            return static_cast<std::size_t>(card.getSlotValues()[placedCubes - 1]);
        }
    }
}
