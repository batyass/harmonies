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
                    int q = cell.offset.getQ();
                    int r = cell.offset.getR();
                    // 60° rotation sens horaire: (q, r) -> (q+r, -q)
                    cell.offset = utils::HexCoord(q + r, -q);
                }
            }
            return Pattern(rotatedCells);
        }

    } // namespace model
} // namespace harmonies