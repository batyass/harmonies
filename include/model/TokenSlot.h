#ifndef TOKENSLOT_H
#define TOKENSLOT_H

#include <cstddef>
#include <vector>
#include "model/TokenType.h"

namespace harmonies
{
    namespace model
    {

        class TokenSlot
        {
        private:
            std::vector<TokenType> tokens;

        public:
            bool isEmpty() const;
            void fill(const std::vector<TokenType> &newTokens);
            std::vector<TokenType> takeAll();
            const std::vector<TokenType> &getTokens() const;
        };

    }
}

#endif
