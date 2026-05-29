#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "core/Game.h"

using harmonies::core::Game;
using harmonies::core::GameState;
using harmonies::model::BoardSide;
using harmonies::model::GameConfig;
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

    {
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
    }

    {
        GameConfig config(2, BoardSide::A, false);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");
        playerNames.push_back("Bob");

        Game game(config, playerNames);
        game.initGame();

        std::vector<TokenType> chosenSlot;
        chosenSlot.push_back(TokenType::BlueWater);
        chosenSlot.push_back(TokenType::GrayStone);
        chosenSlot.push_back(TokenType::BrownEarth);
        game.getCentralBoard()->getSlot(0)->fill(chosenSlot);

        const std::size_t remainingBefore = game.getTokenBag()->getRemainingCount();

        check(game.takeTokensFromSlot(0),
              "Taking tokens from a valid slot should succeed",
              failures);
        check(game.getState() == GameState::WaitingForPlacement,
              "Taking a slot should move the game to WaitingForPlacement",
              failures);
        check(game.getCurrentPlayer() != nullptr &&
                  game.getCurrentPlayer()->getName() == "Alice",
              "Taking a slot should not change the current player immediately",
              failures);
        check(game.getCentralBoard()->getSlot(0) != nullptr &&
                  game.getCentralBoard()->getSlot(0)->getTokens().size() == 3,
              "After taking a slot, it should be refilled to 3 tokens when the bag has enough tokens",
              failures);
        check(game.getTokenBag()->getRemainingCount() == remainingBefore - 3,
              "Refilling the chosen slot should consume 3 tokens from the bag",
              failures);
    }

    {
        GameConfig config(2, BoardSide::A, false);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");
        playerNames.push_back("Bob");

        Game game(config, playerNames);
        game.initGame();

        std::vector<TokenType> chosenSlot;
        chosenSlot.push_back(TokenType::BlueWater);
        chosenSlot.push_back(TokenType::GrayStone);
        chosenSlot.push_back(TokenType::BrownEarth);
        game.getCentralBoard()->getSlot(0)->fill(chosenSlot);

        game.takeTokensFromSlot(0);

        check(game.placeTokenOnBoard(HexCoord(0, 0), TokenType::BlueWater),
              "The first pending token should be placeable on the board",
              failures);
        check(game.getState() == GameState::WaitingForPlacement,
              "The game should stay in WaitingForPlacement while tokens remain to be placed",
              failures);

        check(game.placeTokenOnBoard(HexCoord(1, 0), TokenType::GrayStone),
              "The second pending token should be placeable on the board",
              failures);
        check(game.getState() == GameState::WaitingForPlacement,
              "The game should still wait for placement after the second token",
              failures);

        check(game.placeTokenOnBoard(HexCoord(0, 1), TokenType::BrownEarth),
              "The last pending token should be placeable on the board",
              failures);
        check(game.getState() == GameState::WaitingForSlotChoice,
              "After the last token is placed, the next player should enter the slot-choice phase",
              failures);
        check(game.getCurrentPlayer() != nullptr &&
                  game.getCurrentPlayer()->getName() == "Bob",
              "After placing all pending tokens, the turn should pass to the next player",
              failures);

        check(game.getPlayers()[0]->getBoard()->getCell(HexCoord(0, 0)) != nullptr &&
                  game.getPlayers()[0]->getBoard()->getCell(HexCoord(0, 0))->getHeight() == 1,
              "The first placed token should remain on Alice's board",
              failures);
        check(game.getPlayers()[0]->getBoard()->getCell(HexCoord(1, 0)) != nullptr &&
                  game.getPlayers()[0]->getBoard()->getCell(HexCoord(1, 0))->getHeight() == 1,
              "The second placed token should remain on Alice's board",
              failures);
        check(game.getPlayers()[0]->getBoard()->getCell(HexCoord(0, 1)) != nullptr &&
                  game.getPlayers()[0]->getBoard()->getCell(HexCoord(0, 1))->getHeight() == 1,
              "The third placed token should remain on Alice's board",
              failures);
    }

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
