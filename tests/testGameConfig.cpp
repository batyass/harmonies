#include <iostream>
#include <stdexcept>
#include <string>

#include "model/GameConfig.h"

using harmonies::model::BoardSide;
using harmonies::model::GameConfig;

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

    GameConfig soloConfig(1, BoardSide::A, true);
    GameConfig multiConfig(4, BoardSide::B, false);

    check(soloConfig.getNbPlayer() == 1,
          "A game config should expose the configured player count",
          failures);
    check(soloConfig.getMode() == "solo",
          "A one-player game should be reported as solo",
          failures);
    check(multiConfig.getMode() == "multi",
          "A multiplayer game should be reported as multi",
          failures);
    check(multiConfig.getSide() == BoardSide::B,
          "A game config should expose the configured board side",
          failures);
    check(soloConfig.getNatureSpiritOption(),
          "A game config should expose whether the nature spirit option is enabled",
          failures);

    bool zeroPlayersThrew = false;
    try
    {
        GameConfig invalidConfig(0, BoardSide::A, false);
    }
    catch (const std::invalid_argument &)
    {
        zeroPlayersThrew = true;
    }

    check(zeroPlayersThrew,
          "Constructing a config with zero players should throw",
          failures);

    bool tooManyPlayersThrew = false;
    try
    {
        GameConfig invalidConfig(5, BoardSide::A, false);
    }
    catch (const std::invalid_argument &)
    {
        tooManyPlayersThrew = true;
    }

    check(tooManyPlayersThrew,
          "Constructing a config with more than four players should throw",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
