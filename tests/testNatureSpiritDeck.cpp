#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "model/NatureSpiritDeck.h"

using harmonies::model::NatureSpiritCard;
using harmonies::model::NatureSpiritDeck;
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

    NatureSpiritCard makeCard(const std::string &name)
    {
        Pattern pattern({{HexCoord(0, 0), TokenType::BrownEarth, PatternCell::AnyHeight}});
        return NatureSpiritCard(name, pattern, nullptr);
    }
}

int main()
{
    int failures = 0;

    std::vector<NatureSpiritCard> cards = {
        makeCard("Spirit A"),
        makeCard("Spirit B"),
        makeCard("Spirit C"),
        makeCard("Spirit D")};

    NatureSpiritDeck deck(cards);
    std::vector<std::vector<NatureSpiritCard>> hands = deck.deal(2);

    check(hands.size() == 2,
          "Dealing to two players should return two hands",
          failures);
    check(hands[0].size() == 2 && hands[1].size() == 2,
          "Each player should receive two nature spirit cards",
          failures);

    bool invalidPlayersThrew = false;
    try
    {
        NatureSpiritDeck invalidDeck(cards);
        invalidDeck.deal(0);
    }
    catch (const std::out_of_range &)
    {
        invalidPlayersThrew = true;
    }

    check(invalidPlayersThrew,
          "Dealing to zero players should throw",
          failures);

    bool notEnoughCardsThrew = false;
    try
    {
        std::vector<NatureSpiritCard> tooFewCards = {
            makeCard("Spirit A"),
            makeCard("Spirit B"),
            makeCard("Spirit C")};
        NatureSpiritDeck smallDeck(tooFewCards);
        smallDeck.deal(2);
    }
    catch (const std::out_of_range &)
    {
        notEnoughCardsThrew = true;
    }

    check(notEnoughCardsThrew,
          "Dealing more cards than the deck contains should throw",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
