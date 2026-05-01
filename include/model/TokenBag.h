#ifndef TOKENBAG_H
#define TOKENBAG_H

#include <cstddef>
#include <vector>

#include "model/TokenType.h"

namespace harmonies
{
    namespace model
    {

        class TokenBag
        {
        private:
            std::vector<TokenType> tokens;

        public:
            TokenBag();
            void shuffle();
            std::vector<TokenType> drawTokens(std::size_t count);
            bool isEmpty() const;
            std::size_t getRemainingCount() const;
        };

    }
}

#endif
