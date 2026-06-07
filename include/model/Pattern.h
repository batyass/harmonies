#ifndef PATTERN_H
#define PATTERN_H

#include "utils/hexCoord.h"
#include "model/TokenType.h"
#include <vector>

namespace harmonies
{
    namespace model
    {

        struct PatternCell
        {
            static constexpr int AnyHeight = 0;

            // Relative board coordinate around the anchor cell.
            // This is not a naive q/r delta repeated by multiplication:
            // for a straight line, each cell must use the actual coordinate reached
            // on the even-q board after successive directional steps.
            utils::HexCoord offset;
            TokenType type;         // TokenType en question
            int height;             // Hauteur requise (AnyHeight = n'importe quelle hauteur)
        };

        class Pattern
        {
        private:
            std::vector<PatternCell> cells; // le cube est toujours en (0,0)

        public:
            Pattern(std::vector<PatternCell> cells);

            const std::vector<PatternCell> &getCells() const;

            // Retourne le pattern tourne de (steps * 60°) clockwise
            Pattern rotated(int steps) const;
        };

    } // namespace model
} // namespace harmonies

#endif
