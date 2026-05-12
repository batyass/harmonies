#include "rules/PlacementValidator.h"

namespace harmonies
{
    namespace rules
    {

        bool placementValidator(model::TokenType token, const utils::HexCoord &coord, const model::PersonalBoard &board)
        {
            const model::BoardCell *cell = board.getCell(coord);
            if (cell == nullptr)
                return false;

            if (cell->hasCube())
                return false; // prohibited to place on a cube
            if (cell->getHeight() == 0)
                return true; // empty cell allowed
            if (cell->getHeight() < 3)
            {
                const std::vector<model::TokenType> &stack = cell->getTokenStack();
                std::size_t stackHeight = cell->getHeight();
                if (token == model::TokenType::GrayStone)
                {
                    if (stack[stackHeight - 1] == model::TokenType::GrayStone)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (token == model::TokenType::RedBuilding)
                {
                    if (stackHeight == 1)
                    {
                        if (stack[stackHeight - 1] == model::TokenType::GrayStone || stack[stackHeight - 1] == model::TokenType::RedBuilding || stack[stackHeight - 1] == model::TokenType::BrownEarth)
                            return true;
                    }
                    else
                        return false;
                }
                if (token == model::TokenType::BrownEarth && stackHeight == 1)
                {
                    if (stack[stackHeight - 1] == model::TokenType::BrownEarth)
                        return true;
                    else
                        return false;
                }
                if (token == model::TokenType::GreenTree)
                {
                    if (stack[stackHeight - 1] == model::TokenType::BrownEarth)
                        return true;
                    else
                        return false;
                }
            }
            return false;
        }

    } // namespace rules
} // namespace harmonies
