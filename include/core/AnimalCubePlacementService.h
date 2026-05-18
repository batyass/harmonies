#ifndef ANIMALCUBEPLACEMENTSERVICE_H
#define ANIMALCUBEPLACEMENTSERVICE_H

#include "model/AnimalCard.h"
#include "model/PersonalBoard.h"
#include "utils/hexCoord.h"

namespace harmonies {
    namespace core {

        // The anchor is the board coordinate corresponding to the (0,0) cell of the pattern.
        // In our model, this (0,0) cell is always the one that receives the animal cube.
        bool placeAnimalCube(model::AnimalCard& card,
                            model::PersonalBoard& board,
                            const utils::HexCoord& anchor);

    } // namespace core
} // namespace harmonies

#endif
