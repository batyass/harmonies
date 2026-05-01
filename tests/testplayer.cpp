#include <iostream>
#include <string>

#include "model/Player.h"

using harmonies::model::BoardSide;
using harmonies::model::Player;

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

    Player player("Yassir", BoardSide::A);

    check(player.getName() == "Yassir",
          "A player should expose its name",
          failures);
    check(player.getScore() == 0,
          "A new player should start with a score of 0",
          failures);
    check(player.getBoard() != nullptr,
          "A player should own a personal board",
          failures);
    check(player.getBoard()->getSide() == BoardSide::A,
          "The personal board should keep the side used at construction",
          failures);

    player.addPoints(15);
    check(player.getScore() == 15,
          "Adding positive points should increase the score",
          failures);

    player.addPoints(-4);
    check(player.getScore() == 15,
          "Adding negative points should not decrease the score",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
