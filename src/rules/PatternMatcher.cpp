#include "rules/PatternMatcher.h"
#include "model/BoardCell.h"

namespace harmonies
{
    namespace rules
    {
        bool PatternMatcher::matches(const model::PersonalBoard& board, 
                                   const utils::HexCoord& anchor, 
                                   const model::Pattern& pattern)
        {
            for (const auto& pCell : pattern.getCells()) {
                // Calculate absolute coordinate: anchor + offset
                utils::HexCoord targetCoord(anchor.getQ() + pCell.offset.getQ(), 
                                          anchor.getR() + pCell.offset.getR());

                // 1. Verify coordinate existence
                if (!board.contains(targetCoord)) {
                    return false;
                }

                const model::BoardCell* bCell = board.getCell(targetCoord);
                
                // 2. Verify TokenType (Check the top token of the stack)
                const auto& stack = bCell->getTokenStack();
                if (stack.empty() || stack.back() != pCell.type) {
                    return false;
                }

                // 3. Verify Height requirements
                if (pCell.height != model::PatternCell::AnyHeight) {
                    if (static_cast<int>(bCell->getHeight()) != pCell.height) {
                        return false;
                    }
                }
            }
            return true;
        }

        bool PatternMatcher::matchesAnyRotation(const model::PersonalBoard& board, 
                                               const utils::HexCoord& anchor, 
                                               const model::Pattern& pattern)
        {
            for (int i = 0; i < 6; ++i) {
                if (matches(board, anchor, pattern.rotated(i))) {
                    return true;
                }
            }
            return false;
        }
    }
}