#ifndef ANIMALCARD_H
#define ANIMALCARD_H

#include "model/Pattern.h"
#include <string>
#include <vector>

namespace harmonies
{
    namespace model
    {

        class AnimalCard
        {
        private:
            std::string animalName;
            Pattern pattern;
            std::vector<int> slotValues;
            int cubesOnCard;

        public:
            AnimalCard(std::string name, Pattern pattern, std::vector<int> slotValues);

            const std::string &getName() const;
            const Pattern &getPattern() const;
            const std::vector<int> &getSlotValues() const;

            int getCubesOnCard() const;
            int totalSlots() const;
            bool isComplete() const;

            // Precondition: la carte n'est pas complete.
            int nextSlotValue() const;
            bool placeNextCube();
        };

    } // namespace model
} // namespace harmonies

#endif
