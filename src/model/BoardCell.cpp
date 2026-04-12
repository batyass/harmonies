#include "model/BoardCell.h"

namespace harmonies
{
    namespace model
    {

        BoardCell::BoardCell(utils::HexCoord coord) : coordinate(coord) {}

        bool BoardCell::hasCube() const
        {
            return cubePresent;
        }

        bool BoardCell::canReceiveToken(TokenType type) const
        {
            if (type == TokenType::None)
            {
                return false;
            }

            if (cubePresent)
            {
                return false;
            }

            return tokenStack.size() < 3;
        }

        bool BoardCell::addToken(TokenType type)
        {
            if (!canReceiveToken(type))
            {
                return false;
            }

            tokenStack.push_back(type);
            return true;
        }

        bool BoardCell::canPlaceCube() const
        {
            return !cubePresent;
        }

        bool BoardCell::placeCube()
        {
            if (!canPlaceCube())
            {
                return false;
            }

            cubePresent = true;
            return true;
        }

    } // namespace model
} // namespace harmonies
