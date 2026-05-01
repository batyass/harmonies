#ifndef CENTRALBOARD_H
#define CENTRALBOARD_H

#include <cstddef>
#include <vector>

#include "model/TokenSlot.h"

namespace harmonies
{
    namespace model
    {

        class CentralBoard
        {
        private:
            std::vector<TokenSlot> slots;

        public:
            explicit CentralBoard(int nbPlayers);
            std::size_t getNbSlots() const;
            TokenSlot *getSlot(std::size_t index);
            const TokenSlot *getSlot(std::size_t index) const;
        };

    }
}

#endif
