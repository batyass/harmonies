#include <iostream>
#include <string>

#include "model/BoardCell.h"

using harmonies::model::BoardCell;
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

    BoardCell cell(HexCoord(0, 0));

    check(cell.getHeight() == 0,
          "A new cell should start with height 0",
          failures);
    check(!cell.hasCube(),
          "A new cell should not contain a cube",
          failures);
    check(cell.canPlaceCube(),
          "A new cell should allow cube placement",
          failures);

    check(cell.addToken(TokenType::BlueWater),
          "First token should be accepted",
          failures);
    check(cell.addToken(TokenType::GrayStone),
          "Second token should be accepted",
          failures);
    check(cell.addToken(TokenType::BrownEarth),
          "Third token should be accepted",
          failures);
    check(cell.getHeight() == 3,
          "Height should be 3 after three tokens",
          failures);
    check(!cell.addToken(TokenType::GreenTree),
          "A fourth token should be rejected",
          failures);

    BoardCell cubeCell(HexCoord(1, 0));
    check(cubeCell.placeCube(),
          "First cube placement should succeed",
          failures);
    check(cubeCell.hasCube(),
          "Cell should report the cube after placement",
          failures);
    check(!cubeCell.canPlaceCube(),
          "Second cube placement should be forbidden",
          failures);
    check(!cubeCell.placeCube(),
          "Second cube placement should fail",
          failures);
    check(!cubeCell.addToken(TokenType::YellowField),
          "No token should be accepted once a cube is present",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
