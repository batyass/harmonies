#ifndef NATURESPIRITPLACEMENTSERVICE_H
#define NATURESPIRITPLACEMENTSERVICE_H

#include "model/NatureSpiritCard.h"
#include "model/PersonalBoard.h"
#include "utils/hexCoord.h"

namespace harmonies {
    namespace core {

        // The anchor is the board coordinate corresponding to the (0,0) cell of the pattern.
        // In our model, this (0,0) cell is always the one that receives the nature spirit cube.
        bool placeNatureSpiritCube(model::NatureSpiritCard& card,
                                model::PersonalBoard& board,
                                const utils::HexCoord& anchor);

    } // namespace core
} // namespace harmonies

#endif
