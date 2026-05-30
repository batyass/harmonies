#ifndef PATTERNMATCHER_H
#define PATTERNMATCHER_H

#include "model/PersonalBoard.h"
#include "model/Pattern.h"
#include "utils/hexCoord.h"

namespace harmonies
{
    namespace rules
    {
        /**
         * Checks if the pattern matches at the anchor coordinate in any
         * of its six possible rotations.
         */
        bool matchesAnyRotation(const model::PersonalBoard &board,
                               const utils::HexCoord &anchor,
                               const model::Pattern &pattern);
    }
}

#endif
