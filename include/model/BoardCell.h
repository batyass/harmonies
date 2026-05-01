#ifndef BOARDCELL_H
#define BOARDCELL_H

#include "utils/hexCoord.h"
#include "model/TokenType.h"
#include <cstddef> // size_t
#include <vector>

namespace harmonies
{
    namespace model
    {

        class BoardCell
        {
        private:
            utils::HexCoord coordinate;
            std::vector<TokenType> tokenStack; // Max 3 tokens
            bool cubePresent = false;

        public:
            explicit BoardCell(utils::HexCoord coord);

            // Getters
            const utils::HexCoord &getCoordinate() const { return coordinate; }
            std::size_t getHeight() const { return tokenStack.size(); }

            // Logic for stacking
            bool hasCube() const;
            bool canReceiveToken(TokenType type) const;
            bool addToken(TokenType type);

            bool canPlaceCube() const;
            bool placeCube();
        };

    } // namespace model
} // namespace harmonies

#endif
