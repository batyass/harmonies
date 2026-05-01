#include "model/TokenSlot.h"

namespace harmonies
{
    namespace model
    {

        bool TokenSlot::isEmpty() const
        {
            return tokens.empty();
        }

        void TokenSlot::fill(const std::vector<TokenType> &newTokens)
        {
            tokens = newTokens;
        }

        std::vector<TokenType> TokenSlot::takeAll()
        {
            std::vector<TokenType> toReturn = tokens;
            tokens.clear();
            return toReturn;
        }

        const std::vector<TokenType> &TokenSlot::getTokens() const
        {
            return tokens;
        }

    }
}
