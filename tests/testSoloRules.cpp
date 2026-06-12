#include <iostream>
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
        GameConfig config(1, BoardSide::A, false);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");

        Game game(config, playerNames);
        game.initGame();

        check(game.getAnimalCardDeck()->visibleCount() == 3,
              "In solo, the animal market should expose exactly 3 visible cards",
              failures);
        check(game.canTakeVisibleAnimalCard(),
              "In solo, the player should initially be allowed to take a visible animal card",
              failures);

        std::vector<TokenType> chosenSlot;
        chosenSlot.push_back(TokenType::BlueWater);
        chosenSlot.push_back(TokenType::GrayStone);
        chosenSlot.push_back(TokenType::BrownEarth);
        game.getCentralBoard()->getSlot(0)->fill(chosenSlot);
        game.getCentralBoard()->getSlot(1)->fill(std::vector<TokenType>(3, TokenType::YellowField));
        game.getCentralBoard()->getSlot(2)->fill(std::vector<TokenType>(3, TokenType::RedBuilding));

        const std::size_t bagBeforeTake = game.getTokenBag()->getRemainingCount();
        check(game.takeTokensFromSlot(0),
              "In solo, taking a slot should start the mandatory placement phase",
              failures);
        check(game.getCentralBoard()->getSlot(0)->isEmpty(),
              "In solo, the chosen slot should remain empty during the turn",
              failures);
        check(game.getCentralBoard()->getSlot(1)->getTokens().size() == 3 &&
                  game.getCentralBoard()->getSlot(2)->getTokens().size() == 3,
              "In solo, the unchosen slots should keep their tokens until end of turn",
              failures);
        check(game.getTokenBag()->getRemainingCount() == bagBeforeTake,
              "In solo, taking a slot should not consume bag tokens immediately",
              failures);

        check(game.placeTokenOnBoard(HexCoord(0, 0), TokenType::BlueWater),
              "The first solo mandatory token should be placeable",
              failures);
        check(game.placeTokenOnBoard(HexCoord(1, 0), TokenType::GrayStone),
              "The second solo mandatory token should be placeable",
              failures);
        check(game.placeTokenOnBoard(HexCoord(0, 1), TokenType::BrownEarth),
              "The third solo mandatory token should be placeable",
              failures);
        check(game.getState() == GameState::WaitingForTurnEndChoice,
              "After the three mandatory placements, solo should enter the optional action phase",
              failures);

        const std::size_t bagBeforeEndTurn = game.getTokenBag()->getRemainingCount();
        check(game.endTurn(),
              "After mandatory placements, the solo player should be able to end the turn",
              failures);
        check(game.getCentralBoard()->getSlot(0)->getTokens().size() == 3 &&
                  game.getCentralBoard()->getSlot(1)->getTokens().size() == 3 &&
                  game.getCentralBoard()->getSlot(2)->getTokens().size() == 3,
              "At the end of a solo turn, all 3 market slots should be refilled",
              failures);
        check(game.getTokenBag()->getRemainingCount() == bagBeforeEndTurn - 9,
              "At the end of a solo turn, refilling the 3 slots should consume 9 tokens from the bag",
              failures);
    }

    {
        GameConfig config(1, BoardSide::A, false);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");

        Game game(config, playerNames);
        game.initGame();

        std::vector<TokenType> chosenSlot;
        chosenSlot.push_back(TokenType::BlueWater);
        chosenSlot.push_back(TokenType::GrayStone);
        chosenSlot.push_back(TokenType::BrownEarth);
        game.getCentralBoard()->getSlot(0)->fill(chosenSlot);

        check(game.takeTokensFromSlot(0),
              "In solo, the player should be able to start a turn before testing the discard option",
              failures);
        check(game.placeTokenOnBoard(HexCoord(0, 0), TokenType::BlueWater),
              "The first mandatory solo token should be placeable before discard",
              failures);
        check(game.placeTokenOnBoard(HexCoord(1, 0), TokenType::GrayStone),
              "The second mandatory solo token should be placeable before discard",
              failures);
        check(game.placeTokenOnBoard(HexCoord(0, 1), TokenType::BrownEarth),
              "The third mandatory solo token should be placeable before discard",
              failures);

        check(game.getState() == GameState::WaitingForTurnEndChoice,
              "After placing all solo mandatory tokens, the player should enter the optional action phase",
              failures);
        check(game.canReplaceVisibleAnimalCard(),
              "In solo, the discard option should be available at end of turn if no animal card was taken",
              failures);

        check(game.replaceAnimalCard(0),
              "In solo, the player should be able to discard one visible animal card if none was taken this turn",
              failures);
        check(!game.canReplaceVisibleAnimalCard(),
              "After discarding once in solo, the discard option should no longer be available this turn",
              failures);
        check(!game.canTakeVisibleAnimalCard(),
              "After using the solo discard option, the player should no longer be able to take a visible animal card this turn",
              failures);
        check(!game.takeVisibleAnimalCard(0),
              "The solo discard option should block taking a visible animal card later in the same turn",
              failures);
    }

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
