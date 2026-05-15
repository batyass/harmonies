#include "rules/EndGameChecker.h"

namespace harmonies
{
    namespace rules
    {
        namespace
        {
            bool hasTwoOrFewerEmptySpaces(const model::PersonalBoard &board)
            {
                const std::map<utils::HexCoord, model::BoardCell> &cells = board.getCells();
                int unoccupiedCount = 0;

                for (std::map<utils::HexCoord, model::BoardCell>::const_iterator it = cells.begin(); it != cells.end(); ++it)
                {
                    const model::BoardCell &cell = it->second;
                    if (cell.getHeight() == 0)
                    {
                        ++unoccupiedCount;
                    }
                }

                return unoccupiedCount <= 2;
            }
        }

        bool isGameOver(const std::vector<const model::PersonalBoard *> &boards,
                        const model::TokenBag &bag)
        {
            if (bag.isEmpty())
            {
                return true;
            }

            for (std::size_t i = 0; i < boards.size(); ++i)
            {
                const model::PersonalBoard *board = boards[i];
                if (board != nullptr && hasTwoOrFewerEmptySpaces(*board))
                {
                    return true;
                }
            }

            return false;
        }
    }
}
