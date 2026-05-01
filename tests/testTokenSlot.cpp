#include <iostream>
#include <string>
#include <vector>

#include "model/TokenSlot.h"

using harmonies::model::TokenSlot;
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

    TokenSlot slot;
    std::vector<TokenType> tokens = {TokenType::BlueWater, TokenType::GrayStone, TokenType::BrownEarth};

    check(slot.isEmpty(),
          "A new slot should be empty",
          failures);

    slot.fill(tokens);
    check(!slot.isEmpty(),
          "A filled slot should not be empty",
          failures);
    check(slot.getTokens().size() == 3,
          "A filled slot should expose the inserted tokens",
          failures);
    check(slot.getTokens()[0] == TokenType::BlueWater,
          "The slot should preserve the order of inserted tokens",
          failures);

    std::vector<TokenType> taken = slot.takeAll();
    check(taken.size() == 3,
          "takeAll should return all tokens from the slot",
          failures);
    check(slot.isEmpty(),
          "The slot should be empty after takeAll",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
