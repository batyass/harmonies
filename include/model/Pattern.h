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

            utils::HexCoord offset; // relatif au token contenant le cube (0,0)
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
