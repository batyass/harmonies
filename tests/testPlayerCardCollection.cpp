#include <iostream>
#include <string>

#include "model/PlayerCardCollection.h"

using harmonies::model::AnimalCard;
using harmonies::model::Pattern;
using harmonies::model::PatternCell;
using harmonies::model::PlayerCardCollection;
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

    PlayerCardCollection collection;

    check(collection.getCardCount() == 0,
          "A new player card collection should start empty",
          failures);

    AnimalCard fox = makeCard("Fox", 8);
    AnimalCard bear = makeCard("Bear", 5);

    collection.addCard(fox);
    collection.addCard(bear);

    check(collection.getCardCount() == 2,
          "Adding cards should increase the collection size",
          failures);
    check(collection.getCards()[0].getName() == "Fox",
          "The collection should preserve insertion order",
          failures);

    collection.removeCard(0);
    check(collection.getCardCount() == 1,
          "Removing a card by index should decrease the collection size",
          failures);
    check(collection.getCards()[0].getName() == "Bear",
          "Removing the first card should keep the remaining one accessible",
          failures);

    collection.removeCard(10);
    check(collection.getCardCount() == 1,
          "Removing an invalid index should leave the collection unchanged",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
