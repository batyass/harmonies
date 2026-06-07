#include "model/Pattern.h"

namespace harmonies
{
    namespace model
    {
        Pattern::Pattern(std::vector<PatternCell> cells) : cells(cells) {}

        const std::vector<PatternCell> &Pattern::getCells() const { return cells; }

        Pattern Pattern::rotated(int steps) const
        {
            steps = ((steps % 6) + 6) % 6; // normalise a 0-5 au cas ou
            std::vector<PatternCell> rotatedCells = cells;

            for (int s = 0; s < steps; ++s)
            {
                for (PatternCell &cell : rotatedCells)
                {
                    cell.offset = utils::rotateOffset(cell.offset, 1);
                }
            }
            return Pattern(rotatedCells);
        }

    } // namespace model
} // namespace harmonies
