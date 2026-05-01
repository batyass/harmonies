#include <iostream>
#include <string>

#include "model/NatureSpiritCard.h"

using harmonies::model::NatureSpiritCard;
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

    Pattern pattern({{HexCoord(0, 0), TokenType::BlueWater, PatternCell::AnyHeight}});
    NatureSpiritCard card("Brook Spirit", pattern, nullptr);

    check(card.getName() == "Brook Spirit",
          "A nature spirit card should expose its name",
          failures);
    check(card.getPattern().getCells().size() == 1,
          "A nature spirit card should expose its pattern",
          failures);
    check(card.getEffect() == nullptr,
          "A nature spirit card should keep the provided placeholder effect",
          failures);
    check(!card.isCubePlaced(),
          "A new nature spirit card should start with its cube on the card",
          failures);
    check(card.placeCube(),
          "The first cube placement should succeed",
          failures);
    check(card.isCubePlaced(),
          "The card should report that its cube has been placed",
          failures);
    check(!card.placeCube(),
          "A second cube placement should fail",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
