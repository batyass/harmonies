#include "model/AnimalCard.h"

#include <stdexcept>

namespace harmonies
{
    namespace model
    {

        AnimalCard::AnimalCard(std::string name, Pattern pattern, std::vector<int> slotValues)
            : animalName(name), pattern(pattern), slotValues(slotValues),
              cubesOnCard(slotValues.size()) {}

        const std::string &AnimalCard::getName() const { return animalName; }
        const Pattern &AnimalCard::getPattern() const { return pattern; }
        const std::vector<int> &AnimalCard::getSlotValues() const { return slotValues; }

        int AnimalCard::getCubesOnCard() const { return cubesOnCard; }
        int AnimalCard::totalSlots() const { return slotValues.size(); }
        bool AnimalCard::isComplete() const { return cubesOnCard == 0; }

        int AnimalCard::nextSlotValue() const
        {
            if (isComplete())
            {
                throw std::logic_error("Cannot read next slot value from a completed animal card");
            }

            std::size_t nextIndex = static_cast<std::size_t>(totalSlots() - cubesOnCard);
            return slotValues[nextIndex];
        }

        bool AnimalCard::placeNextCube()
        {
            if (isComplete())
                return false;
            --cubesOnCard;
            return true;
        }

    } // namespace model
} // namespace harmonies
