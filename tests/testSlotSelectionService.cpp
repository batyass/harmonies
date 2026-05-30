#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "core/SlotSelectionService.h"
#include "model/CentralBoard.h"
#include "model/TokenBag.h"
#include "model/TokenType.h"

using harmonies::core::takeTokensFromSlot;
using harmonies::model::CentralBoard;
using harmonies::model::TokenBag;
using harmonies::model::TokenType;

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
        CentralBoard board(2);
        TokenBag bag;
        std::vector<TokenType> initialTokens;
        initialTokens.push_back(TokenType::BlueWater);
        initialTokens.push_back(TokenType::GrayStone);
        initialTokens.push_back(TokenType::BrownEarth);
        board.getSlot(0)->fill(initialTokens);

        std::size_t bagBefore = bag.getRemainingCount();
        std::vector<TokenType> taken = takeTokensFromSlot(board, bag, 0);

        check(taken.size() == 3,
              "Taking a full slot should return its 3 tokens",
              failures);
        check(taken[0] == TokenType::BlueWater &&
                  taken[1] == TokenType::GrayStone &&
                  taken[2] == TokenType::BrownEarth,
              "The service should preserve the order of tokens taken from the slot",
              failures);
        check(board.getSlot(0)->getTokens().size() == 3,
              "A slot should be refilled to 3 tokens when the bag has enough tokens",
              failures);
        check(bag.getRemainingCount() == bagBefore - 3,
              "Refilling a slot should consume 3 tokens from the bag when possible",
              failures);
    }

    {
        CentralBoard board(2);
        TokenBag bag;

        bool threw = false;
        try
        {
            takeTokensFromSlot(board, bag, 42);
        }
        catch (const std::out_of_range &)
        {
            threw = true;
        }

        check(threw,
              "Selecting an invalid slot index should throw",
              failures);
    }

    {
        CentralBoard board(2);
        TokenBag bag;

        bool threw = false;
        try
        {
            takeTokensFromSlot(board, bag, 0);
        }
        catch (const std::logic_error &)
        {
            threw = true;
        }

        check(threw,
              "Selecting an empty slot should throw",
              failures);
    }

    {
        CentralBoard board(2);
        TokenBag bag;
        board.getSlot(0)->fill(std::vector<TokenType>(3, TokenType::YellowField));

        std::size_t bagSize = bag.getRemainingCount();
        bag.drawTokens(bagSize - 2);

        std::vector<TokenType> taken = takeTokensFromSlot(board, bag, 0);

        check(taken.size() == 3,
              "Taking tokens should still return the slot content even when the bag is almost empty",
              failures);
        check(board.getSlot(0)->getTokens().size() == 2,
              "Refilling should use all remaining bag tokens even if fewer than 3 are available",
              failures);
        check(bag.isEmpty(),
              "The bag should be empty after a partial refill that consumes its last tokens",
              failures);
    }

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
