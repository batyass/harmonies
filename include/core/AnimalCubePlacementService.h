#ifndef ANIMALCUBEPLACEMENTSERVICE_H
#define ANIMALCUBEPLACEMENTSERVICE_H

#include "model/AnimalCard.h"
#include "model/PersonalBoard.h"
#include "utils/hexCoord.h"

namespace harmonies {
    namespace core {

        bool placeAnimalCube(model::AnimalCard& card,
                            model::PersonalBoard& board,
                            const utils::HexCoord& anchor);

    } // namespace core
} // namespace harmonies

#endif
