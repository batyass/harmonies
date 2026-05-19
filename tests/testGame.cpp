#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "core/Game.h"

using harmonies::core::Game;
using harmonies::core::GameState;
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

    GameConfig config(2, BoardSide::A, false);
    std::vector<std::string> playerNames;
    playerNames.push_back("Alice");
    playerNames.push_back("Bob");

    Game game(config, playerNames);

    check(game.getState() == GameState::NotStarted,
          "A new game should start in NotStarted state",
          failures);

    check(game.getCurrentPlayer() == nullptr,
          "Current player should be nullptr before initGame",
          failures);

    game.initGame();

    check(game.getState() == GameState::WaitingForSlotChoice,
          "initGame should move the game to WaitingForSlotChoice",
          failures);

    check(game.getCurrentPlayer() != nullptr,
          "Current player should exist after initGame",
          failures);

    check(game.getCurrentPlayer() != nullptr &&
              game.getCurrentPlayer()->getName() == "Alice",
          "The first player should be the current player after initGame",
          failures);

    bool threw = false;
    try
    {
        game.initGame();
    }
    catch (const std::logic_error &)
    {
        threw = true;
    }
    check(threw,
          "Calling initGame twice should throw",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
