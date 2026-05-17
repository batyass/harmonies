#ifndef NATURESPIRITPLACEMENTSERVICE_H
#define NATURESPIRITPLACEMENTSERVICE_H

#include "model/NatureSpiritCard.h"
#include "model/PersonalBoard.h"
#include "utils/hexCoord.h"

namespace harmonies {
    namespace core {

        bool placeNatureSpiritCube(model::NatureSpiritCard& card,
                                model::PersonalBoard& board,
                                const utils::HexCoord& anchor);

    } // namespace core
} // namespace harmonies

#endif
