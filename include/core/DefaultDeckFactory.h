#ifndef DEFAULTDECKFACTORY_H
#define DEFAULTDECKFACTORY_H

#include <cstddef>
#include <vector>

#include "model/AnimalCard.h"
#include "model/NatureSpiritCard.h"

namespace harmonies
{
    namespace core
    {
        std::vector<model::AnimalCard> makeDefaultAnimalCards();
        std::vector<model::NatureSpiritCard> makeDefaultNatureSpiritCards();
        std::size_t visibleAnimalCardSlots(std::size_t playerCount);
    }
}

#endif
