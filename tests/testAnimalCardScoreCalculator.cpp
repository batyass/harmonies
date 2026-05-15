#include <iostream>
#include <string>

#include "model/AnimalCard.h"
#include "scoring/AnimalCardScoreCalculator.h"

using harmonies::model::AnimalCard;
using harmonies::model::Pattern;
using harmonies::model::PatternCell;
using harmonies::model::TokenType;
using harmonies::scoring::AnimalCardScoreCalculator;
using harmonies::utils::HexCoord;

namespace
{
    void check(bool condition, const std::string &message, int &failures)
    {
        if (condition)
        {
            std::cout << "[PASS] " << message << '\n';
        }
        else
        {
            std::cout << "[FAIL] " << message << '\n';
            ++failures;
        }
    }
}

int main()
{
    int failures = 0;

    Pattern pattern({{HexCoord(0, 0), TokenType::BrownEarth, PatternCell::AnyHeight}});
    AnimalCard card("Fox", pattern, {3, 5, 8});

    check(AnimalCardScoreCalculator(card) == 0,
          "A card with all its cubes should score 0 points",
          failures);

    card.placeNextCube();
    check(AnimalCardScoreCalculator(card) == 3,
          "Removing one cube should reveal the first score slot",
          failures);

    card.placeNextCube();
    check(AnimalCardScoreCalculator(card) == 5,
          "Removing two cubes should reveal the second score slot",
          failures);

    card.placeNextCube();
    check(AnimalCardScoreCalculator(card) == 8,
          "A completed card should score the top slot value",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
