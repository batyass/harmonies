#ifndef TOKENSLOT_H
#define TOKENSLOT_H

#include <vector>
#include "model/Token.h"

namespace harmonies
{
    namespace model
    {

        class TokenSlot
        {
        private:
            std::vector<Token *> tokens;
            bool available;

        public:
            TokenSlot();
            void fill(const std::vector<Token *> &newTokens);
            std::vector<Token *> takeAll();
            bool isAvailable() const { return available; }
        };

    }
}

#endif