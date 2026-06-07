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
            model::Pattern makePattern(std::initializer_list<model::PatternCell> cells)
            {
                return model::Pattern(std::vector<model::PatternCell>(cells));
            }
        }

        std::vector<model::AnimalCard> makeDefaultAnimalCards()
        {
            std::vector<model::AnimalCard> cards;

            // Convention: stable lowercase names, with underscores only when needed.
            // These names are intended to stay machine-friendly so the UI can derive
            // asset paths such as cards/animal/<name>.png without extra translation.
            //
            // The six cards below now match the actual illustrated motifs used in the
            // Qt assets instead of the previous single-cell placeholder patterns.
            cards.push_back(model::AnimalCard(
                "fennec",
                makePattern({
                    {utils::HexCoord(-2, 0), model::TokenType::YellowField, 1},
                    {utils::HexCoord(-1, 0), model::TokenType::GrayStone, 1},
                    {utils::HexCoord(0, 0), model::TokenType::GrayStone, 1},
                }),
                std::vector<int>{4, 9, 16}));

            cards.push_back(model::AnimalCard(
                "raven",
                makePattern({
                    {utils::HexCoord(-1, 1), model::TokenType::RedBuilding, 2},
                    {utils::HexCoord(0, 0), model::TokenType::YellowField, 1},
                    {utils::HexCoord(1, 1), model::TokenType::RedBuilding, 2},
                }),
                std::vector<int>{4, 9}));

            cards.push_back(model::AnimalCard(
                "squirrel",
                makePattern({
                    {utils::HexCoord(-1, 0), model::TokenType::GreenTree, 3},
                    {utils::HexCoord(0, 0), model::TokenType::RedBuilding, 2},
                }),
                std::vector<int>{4, 9, 15}));

            cards.push_back(model::AnimalCard(
                "parrot",
                makePattern({
                    {utils::HexCoord(-1, 0), model::TokenType::BlueWater, 1},
                    {utils::HexCoord(-1, 1), model::TokenType::BlueWater, 1},
                    {utils::HexCoord(0, 0), model::TokenType::GreenTree, 2},
                }),
                std::vector<int>{4, 9, 14}));

            cards.push_back(model::AnimalCard(
                "duck",
                makePattern({
                    {utils::HexCoord(-1, 0), model::TokenType::RedBuilding, 2},
                    {utils::HexCoord(0, 0), model::TokenType::BlueWater, 1},
                }),
                std::vector<int>{2, 4, 8, 13}));

            cards.push_back(model::AnimalCard(
                "fox",
                makePattern({
                    {utils::HexCoord(-2, 0), model::TokenType::GreenTree, 2},
                    {utils::HexCoord(-1, 0), model::TokenType::BlueWater, 1},
                    {utils::HexCoord(0, 0), model::TokenType::GrayStone, 3},
                }),
                std::vector<int>{2, 4, 9, 16}));

            return cards;
        }

        std::vector<model::NatureSpiritCard> makeDefaultNatureSpiritCards()
        {
            std::vector<model::NatureSpiritCard> cards;
            scoring::NatureSpiritEffect effect;

            cards.push_back(model::NatureSpiritCard("brook_spirit", makePattern({{utils::HexCoord(0, 0), model::TokenType::BlueWater, model::PatternCell::AnyHeight}}), effect));
            cards.push_back(model::NatureSpiritCard("peak_spirit", makePattern({{utils::HexCoord(0, 0), model::TokenType::GrayStone, model::PatternCell::AnyHeight}}), effect));
            cards.push_back(model::NatureSpiritCard("grove_spirit", makePattern({{utils::HexCoord(0, 0), model::TokenType::GreenTree, model::PatternCell::AnyHeight}}), effect));
            cards.push_back(model::NatureSpiritCard("field_spirit", makePattern({{utils::HexCoord(0, 0), model::TokenType::YellowField, model::PatternCell::AnyHeight}}), effect));
            cards.push_back(model::NatureSpiritCard("clay_spirit", makePattern({{utils::HexCoord(0, 0), model::TokenType::BrownEarth, model::PatternCell::AnyHeight}}), effect));
            cards.push_back(model::NatureSpiritCard("tower_spirit", makePattern({{utils::HexCoord(0, 0), model::TokenType::RedBuilding, model::PatternCell::AnyHeight}}), effect));
            cards.push_back(model::NatureSpiritCard("spring_spirit", makePattern({{utils::HexCoord(0, 0), model::TokenType::BlueWater, model::PatternCell::AnyHeight}}), effect));
            cards.push_back(model::NatureSpiritCard("forest_spirit", makePattern({{utils::HexCoord(0, 0), model::TokenType::GreenTree, model::PatternCell::AnyHeight}}), effect));

            return cards;
        }

        std::size_t visibleAnimalCardSlots(std::size_t playerCount)
        {
            return playerCount == 1 ? 3 : 5;
        }
    }
}
