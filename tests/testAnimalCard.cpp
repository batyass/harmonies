#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "model/AnimalCard.h"

using harmonies::model::AnimalCard;
using harmonies::model::Pattern;
using harmonies::model::PatternCell;
using harmonies::model::TokenType;
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
    AnimalCard card("Fox", pattern, {8, 5, 3});

    check(card.getName() == "Fox",
          "An animal card should expose its name",
          failures);
    check(card.getPattern().getCells().size() == 1,
          "An animal card should expose its pattern",
          failures);
    check(card.getCubesOnCard() == 3,
          "A new card should start with all cubes on the card",
          failures);
    check(card.nextSlotValue() == 8,
          "The next slot value should start from the highest score",
          failures);

    check(card.placeNextCube(),
          "The first cube placement should succeed",
          failures);
    check(card.getCubesOnCard() == 2,
          "Placing a cube should decrement the remaining cube count",
          failures);
    check(card.nextSlotValue() == 5,
          "The next slot value should advance after a placement",
          failures);

    card.placeNextCube();
    card.placeNextCube();
    check(card.isComplete(),
          "The card should be complete after all cubes are placed",
          failures);
    check(!card.placeNextCube(),
          "No extra cube placement should be allowed on a completed card",
          failures);

    bool threw = false;
    try
    {
        card.nextSlotValue();
    }
    catch (const std::logic_error &)
    {
        threw = true;
    }

    check(threw,
          "Requesting the next slot value on a completed card should throw",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
