#include "model/CentralBoard.h"

namespace harmonies
{
    namespace model
    {

        CentralBoard::CentralBoard(int nbPlayers)
        {
            std::size_t nbSlots = (nbPlayers == 1) ? 3 : 5;
            slots.resize(nbSlots);
        }

        std::size_t CentralBoard::getNbSlots() const
        {
            return slots.size();
        }

        TokenSlot *CentralBoard::getSlot(std::size_t index)
        {
            if (index >= slots.size())
            {
                return nullptr;
            }

            return &slots[index];
        }

        const TokenSlot *CentralBoard::getSlot(std::size_t index) const
        {
            if (index >= slots.size())
            {
                return nullptr;
            }

            return &slots[index];
        }

    }
}
