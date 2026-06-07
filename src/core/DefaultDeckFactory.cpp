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

            // Convention: stable lowercase names, with underscores only when needed.
            // These names are intended to stay machine-friendly so the UI can derive
            // asset paths such as cards/animal/<name>.png without extra translation.
            cards.push_back(model::AnimalCard("fox", singleCellPattern(model::TokenType::BrownEarth), std::vector<int>(1, 3)));
            cards.push_back(model::AnimalCard("bear", singleCellPattern(model::TokenType::GrayStone), std::vector<int>(1, 4)));
            cards.push_back(model::AnimalCard("deer", singleCellPattern(model::TokenType::GreenTree), std::vector<int>(1, 5)));
            cards.push_back(model::AnimalCard("hedgehog", singleCellPattern(model::TokenType::YellowField), std::vector<int>(1, 6)));
            cards.push_back(model::AnimalCard("otter", singleCellPattern(model::TokenType::BlueWater), std::vector<int>(1, 7)));
            cards.push_back(model::AnimalCard("rabbit", singleCellPattern(model::TokenType::RedBuilding), std::vector<int>(1, 8)));

            return cards;
        }

        std::vector<model::NatureSpiritCard> makeDefaultNatureSpiritCards()
        {
            std::vector<model::NatureSpiritCard> cards;
            scoring::NatureSpiritEffect effect;

            cards.push_back(model::NatureSpiritCard("brook_spirit", singleCellPattern(model::TokenType::BlueWater), effect));
            cards.push_back(model::NatureSpiritCard("peak_spirit", singleCellPattern(model::TokenType::GrayStone), effect));
            cards.push_back(model::NatureSpiritCard("grove_spirit", singleCellPattern(model::TokenType::GreenTree), effect));
            cards.push_back(model::NatureSpiritCard("field_spirit", singleCellPattern(model::TokenType::YellowField), effect));
            cards.push_back(model::NatureSpiritCard("clay_spirit", singleCellPattern(model::TokenType::BrownEarth), effect));
            cards.push_back(model::NatureSpiritCard("tower_spirit", singleCellPattern(model::TokenType::RedBuilding), effect));
            cards.push_back(model::NatureSpiritCard("spring_spirit", singleCellPattern(model::TokenType::BlueWater), effect));
            cards.push_back(model::NatureSpiritCard("forest_spirit", singleCellPattern(model::TokenType::GreenTree), effect));

            return cards;
        }

        std::size_t visibleAnimalCardSlots(std::size_t playerCount)
        {
            return playerCount == 1 ? 3 : 5;
        }
    }
}
