#include "core/DefaultDeckFactory.h"

#include <vector>

#include "scoring/NatureSpiritEffect.h"
#include "utils/hexCoord.h"

namespace harmonies
{
    namespace core
    {
        namespace
        {
            model::Pattern singleCellPattern(model::TokenType type)
            {
                return model::Pattern(std::vector<model::PatternCell>(
                    1, model::PatternCell{utils::HexCoord(0, 0), type, model::PatternCell::AnyHeight}));
            }
        }

        std::vector<model::AnimalCard> makeDefaultAnimalCards()
        {
            std::vector<model::AnimalCard> cards;

            cards.push_back(model::AnimalCard("Fox", singleCellPattern(model::TokenType::BrownEarth), std::vector<int>(1, 3)));
            cards.push_back(model::AnimalCard("Bear", singleCellPattern(model::TokenType::GrayStone), std::vector<int>(1, 4)));
            cards.push_back(model::AnimalCard("Deer", singleCellPattern(model::TokenType::GreenTree), std::vector<int>(1, 5)));
            cards.push_back(model::AnimalCard("Hedgehog", singleCellPattern(model::TokenType::YellowField), std::vector<int>(1, 6)));
            cards.push_back(model::AnimalCard("Otter", singleCellPattern(model::TokenType::BlueWater), std::vector<int>(1, 7)));
            cards.push_back(model::AnimalCard("Rabbit", singleCellPattern(model::TokenType::RedBuilding), std::vector<int>(1, 8)));

            return cards;
        }

        std::vector<model::NatureSpiritCard> makeDefaultNatureSpiritCards()
        {
            std::vector<model::NatureSpiritCard> cards;
            scoring::NatureSpiritEffect effect;

            cards.push_back(model::NatureSpiritCard("Brook Spirit", singleCellPattern(model::TokenType::BlueWater), effect));
            cards.push_back(model::NatureSpiritCard("Peak Spirit", singleCellPattern(model::TokenType::GrayStone), effect));
            cards.push_back(model::NatureSpiritCard("Grove Spirit", singleCellPattern(model::TokenType::GreenTree), effect));
            cards.push_back(model::NatureSpiritCard("Field Spirit", singleCellPattern(model::TokenType::YellowField), effect));
            cards.push_back(model::NatureSpiritCard("Clay Spirit", singleCellPattern(model::TokenType::BrownEarth), effect));
            cards.push_back(model::NatureSpiritCard("Tower Spirit", singleCellPattern(model::TokenType::RedBuilding), effect));
            cards.push_back(model::NatureSpiritCard("Spring Spirit", singleCellPattern(model::TokenType::BlueWater), effect));
            cards.push_back(model::NatureSpiritCard("Forest Spirit", singleCellPattern(model::TokenType::GreenTree), effect));

            return cards;
        }

        std::size_t visibleAnimalCardSlots(std::size_t playerCount)
        {
            return playerCount == 1 ? 3 : 5;
        }
    }
}
