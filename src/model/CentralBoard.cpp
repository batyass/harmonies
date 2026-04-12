#include "model/CentralBoard.h"

namespace harmonies
{
    namespace model
    {

        CentralBoard::CentralBoard(int nbPlayers)
        {
            nbSlots = (nbPlayers == 1) ? 3 : 5;
            for (int i = 0; i < nbSlots; ++i)
            {
                slots.push_back(new TokenSlot());
            }
        }

        CentralBoard::~CentralBoard()
        {
            for (auto s : slots)
                delete s;
        }

    }
}