#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "model/AnimalCardDeck.h"

using harmonies::model::AnimalCard;
using harmonies::model::AnimalCardDeck;
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

    AnimalCard makeCard(const std::string &name, int score)
    {
        Pattern pattern({{HexCoord(0, 0), TokenType::BrownEarth, PatternCell::AnyHeight}});
        return AnimalCard(name, pattern, {score});
    }
}

int main()
{
    int failures = 0;

    std::vector<AnimalCard> cards = {
        makeCard("Fox", 8),
        makeCard("Bear", 7),
        makeCard("Deer", 6),
        makeCard("Hedgehog", 5)};

    AnimalCardDeck soloDeck(cards, 3);

    check(soloDeck.visibleCount() == 3,
          "A solo deck should reveal 3 cards at initialization",
          failures);
    check(!soloDeck.drawPileEmpty(),
          "The draw pile should still contain hidden cards if not all cards are visible",
          failures);

    AnimalCard taken = soloDeck.takeVisible(0);
    check(soloDeck.visibleCount() == 2,
          "Taking a visible card should reduce the visible count",
          failures);
    check(!taken.getName().empty(),
          "Taking a visible card should return a valid animal card",
          failures);

    soloDeck.refill();
    check(soloDeck.visibleCount() == 3,
          "Refill should restore the visible cards up to the configured number of slots",
          failures);

    bool invalidSlotsThrew = false;
    try
    {
        AnimalCardDeck invalidDeck(cards, 4);
    }
    catch (const std::out_of_range &)
    {
        invalidSlotsThrew = true;
    }

    check(invalidSlotsThrew,
          "Constructing a deck with an invalid visible slot count should throw",
          failures);

    bool invalidIndexThrew = false;
    try
    {
        soloDeck.takeVisible(10);
    }
    catch (const std::out_of_range &)
    {
        invalidIndexThrew = true;
    }

    check(invalidIndexThrew,
          "Taking a card with an invalid visible index should throw",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
