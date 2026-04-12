#include <iostream>
#include <string>

#include "model/PersonalBoard.h"

using harmonies::model::BoardCell;
using harmonies::model::BoardSide;
using harmonies::model::PersonalBoard;
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

    PersonalBoard board(BoardSide::A);
    HexCoord center(0, 0);
    HexCoord outside(4, 0);

    check(board.contains(center),
          "Center cell should exist on the board",
          failures);
    check(!board.contains(outside),
          "A coordinate outside the radius should not exist",
          failures);
    check(board.getCell(center) != nullptr,
          "getCell should return a valid pointer for an existing cell",
          failures);
    check(board.getCell(outside) == nullptr,
          "getCell should return nullptr outside the board",
          failures);

    check(board.placeToken(center, TokenType::BlueWater),
          "Placing a token on a valid cell should succeed",
          failures);
    check(board.placeToken(center, TokenType::GrayStone),
          "Second token on the same valid cell should succeed",
          failures);
    check(board.placeToken(center, TokenType::BrownEarth),
          "Third token on the same valid cell should succeed",
          failures);
    check(!board.placeToken(center, TokenType::GreenTree),
          "Fourth token on the same cell should fail",
          failures);
    check(!board.placeToken(outside, TokenType::YellowField),
          "Placing a token outside the board should fail",
          failures);

    BoardCell *cell = board.getCell(center);
    check(cell != nullptr && cell->getHeight() == 3,
          "Board cell height should reflect token placements",
          failures);
    check(cell != nullptr && cell->placeCube(),
          "A cube should be placeable on a valid cell",
          failures);
    check(cell != nullptr && !board.placeToken(center, TokenType::YellowField),
          "No token should be accepted after a cube is placed on the cell",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
