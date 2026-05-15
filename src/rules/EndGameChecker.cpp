#include "rules/EndGameChecker.h"
#include "model/BoardCell.h"

namespace harmonies
{
    namespace rules
    {
        bool EndGameChecker::isGameOver(const std::vector<model::Player*>& players, 
                                      const model::TokenBag& bag)
        {
            // Condition 1: Bag is empty
            if (bag.isEmpty()) {
                return true;
            }

            // Condition 2: Any player has 2 or fewer unoccupied cells
            for (const auto* player : players) {
                if (player != nullptr && hasTwoOrFewerEmptySpaces(*(player->getBoard()))) {
                    return true;
                }
            }

            return false;
        }

        bool EndGameChecker::hasTwoOrFewerEmptySpaces(const model::PersonalBoard& board)
        {
            const auto& cells = board.getCells();
            int unoccupiedCount = 0;

            // Count cells with 0 height (inoccupées)
            for (auto const& [coord, cell] : cells) {
                if (cell.getHeight() == 0) {
                    unoccupiedCount++;
                }
            }

            // Rule: 2 unoccupied cells or less
            return unoccupiedCount <= 2;
        }
    }
}