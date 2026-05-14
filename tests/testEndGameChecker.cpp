#include <iostream>
#include <string>
#include <vector>

#include "rules/EndGameChecker.h"
#include "model/Player.h"
#include "model/TokenBag.h"
#include "model/BoardSide.h"
#include "model/TokenType.h"

using harmonies::model::Player;
using harmonies::model::TokenBag;
using harmonies::model::BoardSide;
using harmonies::model::TokenType;
using harmonies::rules::EndGameChecker;

namespace
{
    /**
     * Helper to verify test results.
     */
    void check(bool condition, const std::string &message, int &failures)
    {
        if (condition) {
            std::cout << "[PASS] " << message << '\n';
        } else {
            std::cout << "[FAIL] " << message << '\n';
            ++failures;
        }
    }
}

int main()
{
    int failures = 0;

    // Setup: Create player and bag
    TokenBag bag;
    Player player1("Peilin", BoardSide::A);
    std::vector<Player*> players = { &player1 };

    // Test 1: Initial state
    check(!EndGameChecker::isGameOver(players, bag),
          "Game should not be over at the start",
          failures);

    // Test 2: Bag empty condition
    while (!bag.isEmpty()) {
        bag.drawTokens(1);
    }
    check(EndGameChecker::isGameOver(players, bag),
          "Game should be over when bag is empty",
          failures);

    // Test 3: Threshold of unoccupied cells (Rule: 2 or fewer)
    TokenBag fullBag; // Reset bag for next test
    auto* board = player1.getBoard();
    auto& cells = board->getCells();
    
    // Fill the board until only 3 empty spaces remain
    int fillCount = 0;
    int totalCells = static_cast<int>(cells.size());
    for (auto const& [coord, cell] : cells) {
        if (fillCount < totalCells - 3) {
            board->placeToken(coord, TokenType::GrayStone);
        }
        fillCount++;
    }

    // Verify 3 empty spaces do NOT trigger game over
    check(!EndGameChecker::isGameOver(players, fullBag),
          "Game should NOT be over when 3 empty spaces remain",
          failures);

    // Place one more token to leave exactly 2 empty spaces
    for (auto const& [coord, cell] : cells) {
        if (board->getCell(coord)->getHeight() == 0) {
            board->placeToken(coord, TokenType::GrayStone);
            break;
        }
    }

    // Verify 2 empty spaces DO trigger game over
    check(EndGameChecker::isGameOver(players, fullBag),
          "Game should be over when 2 empty spaces remain",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}