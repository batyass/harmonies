#include <iostream>
#include <string>

#include "model/TokenBag.h"

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

    TokenBag bag;
    std::size_t initialCount = bag.getRemainingCount();

    check(initialCount == 120,
          "A new bag should contain 120 terrain tokens",
          failures);
    check(!bag.isEmpty(),
          "A new bag should not be empty",
          failures);

    std::vector<TokenType> firstDraw = bag.drawTokens(4);
    check(firstDraw.size() == 4,
          "Drawing 4 tokens should return 4 tokens when the bag is full",
          failures);
    check(bag.getRemainingCount() == initialCount - 4,
          "The bag count should decrease after drawing tokens",
          failures);

    std::vector<TokenType> lastDraw = bag.drawTokens(200);
    check(lastDraw.size() == 116,
          "Drawing more tokens than available should return only the remaining tokens",
          failures);
    check(bag.isEmpty(),
          "The bag should be empty after drawing all remaining tokens",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
