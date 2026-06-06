#include "model/CentralBoard.h"

namespace harmonies
{
    namespace model
    {

        CentralBoard::CentralBoard(int nbPlayers)
        {
            std::size_t nbSlots = (nbPlayers == 1) ? 3 : 5;
            marketSlots.resize(nbSlots);
        }

        std::size_t CentralBoard::getNbSlots() const
        {
            return marketSlots.size();
        }

        TokenSlot *CentralBoard::getSlot(std::size_t index)
        {
            if (index >= marketSlots.size())
            {
                return nullptr;
            }

            return &marketSlots[index];
        }

        const TokenSlot *CentralBoard::getSlot(std::size_t index) const
        {
            if (index >= marketSlots.size())
            {
                return nullptr;
            }

            return &marketSlots[index];
        }

    }
}
