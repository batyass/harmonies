#include "rules/PatternMatcher.h"

namespace harmonies
{
    namespace rules
    {
        namespace
        {
            bool matches(const model::PersonalBoard &board,
                         const utils::HexCoord &anchor,
                         const model::Pattern &pattern)
            {
                const std::vector<model::PatternCell> &patternCells = pattern.getCells();

                for (std::size_t i = 0; i < patternCells.size(); ++i)
                {
                    const model::PatternCell &pCell = patternCells[i];
                    // Calculate absolute coordinate: anchor + offset
                    utils::HexCoord targetCoord(anchor.getQ() + pCell.offset.getQ(),
                                                anchor.getR() + pCell.offset.getR());

                    // 1. Verify coordinate existence
                    if (!board.contains(targetCoord))
                    {
                        return false;
                    }

                    const model::BoardCell *bCell = board.getCell(targetCoord);

                    // 2. Verify TokenType (Check the top token of the stack)
                    const std::vector<model::TokenType> &stack = bCell->getTokenStack();
                    if (stack.empty() || stack.back() != pCell.type)
                    {
                        return false;
                    }

                    // 3. Verify Height requirements
                    if (pCell.height != model::PatternCell::AnyHeight)
                    {
                        if (static_cast<int>(bCell->getHeight()) != pCell.height)
                        {
                            return false;
                        }
                    }
                }

                return true;
            }
        }

        bool matchesAnyRotation(const model::PersonalBoard &board,
                                const utils::HexCoord &anchor,
                                const model::Pattern &pattern)
        {
            for (int i = 0; i < 6; ++i)
            {
                if (matches(board, anchor, pattern.rotated(i)))
                {
                    return true;
                }
            }
            return false;
        }
    }
}
