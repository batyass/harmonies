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

            utils::HexCoord step(utils::HexDirection direction, int distance)
            {
                return utils::advanceInDirection(utils::HexCoord(0, 0), direction, distance);
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
                    {step(utils::HexDirection::UpLeft, 2), model::TokenType::YellowField, 1},
                    {step(utils::HexDirection::UpLeft, 1), model::TokenType::GrayStone, 1},
                    {utils::HexCoord(0, 0), model::TokenType::GrayStone, 1},
                }),
                std::vector<int>{4, 9, 16}));

            cards.push_back(model::AnimalCard(
                "raven",
                makePattern({
                    {utils::HexCoord(1, 0), model::TokenType::RedBuilding, 2},
                    {utils::HexCoord(0, 0), model::TokenType::YellowField, 1},
                    {utils::HexCoord(0, 1), model::TokenType::RedBuilding, 2},
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
                    {step(utils::HexDirection::UpLeft, 2), model::TokenType::GreenTree, 2},
                    {step(utils::HexDirection::UpLeft, 1), model::TokenType::BlueWater, 1},
                    {utils::HexCoord(0, 0), model::TokenType::GrayStone, 3},
                }),
                std::vector<int>{2, 4, 9, 16}));

            return cards;
        }

        std::vector<model::NatureSpiritCard> makeDefaultNatureSpiritCards()
        {
            std::vector<model::NatureSpiritCard> cards;

            auto connectedEffect = [](std::initializer_list<scoring::ConnectedGroupRule> rules) {
                scoring::NatureSpiritEffect effect;
                effect.type = scoring::NSEffectType::CountConnectedGroups;
                effect.connectedRules = std::vector<scoring::ConnectedGroupRule>(rules);
                return effect;
            };

            cards.push_back(model::NatureSpiritCard(
                "brook",
                makePattern({
                    {utils::HexCoord(-2, -1), model::TokenType::BlueWater, 1},
                    {utils::HexCoord(-1, 0), model::TokenType::BlueWater, 1},
                    {utils::HexCoord(0, 0), model::TokenType::YellowField, 1},
                }),
                connectedEffect({
                    {model::TokenType::BlueWater, 1, 1, 2},
                    {model::TokenType::BlueWater, 2, 2, 5},
                    {model::TokenType::BlueWater, 3, 0, 9},
                })));

            cards.push_back(model::NatureSpiritCard(
                "clay",
                makePattern({
                    {utils::HexCoord(0, 0), model::TokenType::GreenTree, 2},
                    {utils::HexCoord(1, 0), model::TokenType::RedBuilding, 1},
                }),
                connectedEffect({
                    {model::TokenType::BrownEarth, 1, 1, 4},
                    {model::TokenType::BrownEarth, 2, 2, 8},
                    {model::TokenType::BrownEarth, 3, 0, 12},
                })));

            cards.push_back(model::NatureSpiritCard(
                "field",
                makePattern({
                    {utils::HexCoord(-1, 0), model::TokenType::YellowField, 1},
                    {utils::HexCoord(0, 0), model::TokenType::YellowField, 1},
                    {utils::HexCoord(1, 1), model::TokenType::YellowField, 1},
                }),
                connectedEffect({
                    {model::TokenType::YellowField, 1, 1, 2},
                    {model::TokenType::YellowField, 2, 2, 4},
                    {model::TokenType::YellowField, 3, 0, 8},
                })));

            cards.push_back(model::NatureSpiritCard(
                "grove",
                makePattern({
                    {utils::HexCoord(0, 0), model::TokenType::GreenTree, 2},
                    {utils::HexCoord(1, 0), model::TokenType::GreenTree, 1},
                    {utils::HexCoord(2, -1), model::TokenType::YellowField, 1},
                }),
                connectedEffect({
                    {model::TokenType::GreenTree, 1, 1, 3},
                    {model::TokenType::GreenTree, 2, 2, 6},
                    {model::TokenType::GreenTree, 3, 0, 10},
                })));

            cards.push_back(model::NatureSpiritCard(
                "lion",
                makePattern({
                    {utils::HexCoord(-1, 0), model::TokenType::GreenTree, 2},
                    {utils::HexCoord(0, 0), model::TokenType::YellowField, 1},
                    {utils::HexCoord(1, 1), model::TokenType::YellowField, 1},
                }),
                connectedEffect({
                    {model::TokenType::YellowField, 1, 2, 2},
                    {model::TokenType::YellowField, 3, 0, 10},
                })));

            cards.push_back(model::NatureSpiritCard(
                "peak",
                makePattern({
                    {utils::HexCoord(0, 0), model::TokenType::GrayStone, 3},
                    {utils::HexCoord(1, 0), model::TokenType::GrayStone, 2},
                }),
                connectedEffect({
                    {model::TokenType::GrayStone, 1, 1, 4},
                    {model::TokenType::GrayStone, 2, 2, 7},
                    {model::TokenType::GrayStone, 3, 0, 11},
                })));

            cards.push_back(model::NatureSpiritCard(
                "spring",
                makePattern({
                    {utils::HexCoord(1, 0), model::TokenType::BlueWater, 1},
                    {utils::HexCoord(0, 0), model::TokenType::GreenTree, 1},
                    {utils::HexCoord(0, 1), model::TokenType::BlueWater, 1},
                }),
                connectedEffect({
                    {model::TokenType::BlueWater, 1, 1, 3},
                    {model::TokenType::BlueWater, 2, 2, 7},
                    {model::TokenType::BlueWater, 3, 0, 12},
                })));

            cards.push_back(model::NatureSpiritCard(
                "tower",
                makePattern({
                    {utils::HexCoord(-1, 0), model::TokenType::RedBuilding, 2},
                    {utils::HexCoord(0, 0), model::TokenType::YellowField, 1},
                    {utils::HexCoord(1, 1), model::TokenType::RedBuilding, 2},
                }),
                connectedEffect({
                    {model::TokenType::RedBuilding, 1, 1, 5},
                    {model::TokenType::RedBuilding, 2, 2, 9},
                    {model::TokenType::RedBuilding, 3, 0, 14},
                })));

            return cards;
        }

        std::size_t visibleAnimalCardSlots(std::size_t playerCount)
        {
            return playerCount == 1 ? 3 : 5;
        }
    }
}
