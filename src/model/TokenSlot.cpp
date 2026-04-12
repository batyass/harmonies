#include "model/TokenSlot.h"

namespace harmonies
{
    namespace model
    {

        TokenSlot::TokenSlot() : available(false) {}

        void TokenSlot::fill(const std::vector<Token *> &newTokens)
        {
            tokens = newTokens;
            available = true;
        }

        std::vector<Token *> TokenSlot::takeAll()
        {
            std::vector<Token *> toReturn = tokens;
            tokens.clear();
            available = false;
            return toReturn;
        }

    }
}