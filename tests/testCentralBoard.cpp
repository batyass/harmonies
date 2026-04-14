#include <iostream>
#include <string>

#include "model/CentralBoard.h"

using harmonies::model::CentralBoard;
using harmonies::model::TokenSlot;

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

    CentralBoard soloBoard(1);
    CentralBoard multiBoard(2);

    check(soloBoard.getNbSlots() == 3,
          "A solo central board should contain 3 slots",
          failures);
    check(multiBoard.getNbSlots() == 5,
          "A multiplayer central board should contain 5 slots",
          failures);

    TokenSlot *firstSlot = soloBoard.getSlot(0);
    check(firstSlot != nullptr,
          "getSlot should return a valid pointer for an existing slot",
          failures);
    check(soloBoard.getSlot(3) == nullptr,
          "getSlot should return nullptr outside the board bounds",
          failures);

    const CentralBoard &constBoard = multiBoard;
    check(constBoard.getSlot(4) != nullptr,
          "The const overload of getSlot should work on a valid index",
          failures);
    check(constBoard.getSlot(5) == nullptr,
          "The const overload of getSlot should return nullptr out of bounds",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
