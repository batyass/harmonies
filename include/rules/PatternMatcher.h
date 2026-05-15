#ifndef PATTERNMATCHER_H
#define PATTERNMATCHER_H

#include "model/PersonalBoard.h"
#include "model/Pattern.h"
#include "utils/hexCoord.h"

namespace harmonies
{
    namespace rules
    {
        class PatternMatcher
        {
        public:
            /**
             * Checks if the specified pattern matches at the given anchor coordinate 
             * on the board. The anchor corresponds to the (0,0) coordinate in the Pattern.
             */
            static bool matches(const model::PersonalBoard& board, 
                               const utils::HexCoord& anchor, 
                               const model::Pattern& pattern);

            /**
             * Checks if the pattern matches at the anchor coordinate in any 
             * of its six possible rotations.
             */
            static bool matchesAnyRotation(const model::PersonalBoard& board, 
                                          const utils::HexCoord& anchor, 
                                          const model::Pattern& pattern);
        };
    }
}

#endif