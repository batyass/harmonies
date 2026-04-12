#ifndef CENTRALBOARD_H
#define CENTRALBOARD_H

#include <vector>
#include "TokenSlot.h"

namespace harmonies
{
    namespace model
    {

        class CentralBoard
        {
        private:
            std::vector<TokenSlot *> slots;
            int nbSlots;

        public:
            CentralBoard(int nbPlayers);
            ~CentralBoard();
            int getNbSlots() const { return nbSlots; }
            TokenSlot *getSlot(int index) { return slots[index]; }
        };

    }
}

#endif