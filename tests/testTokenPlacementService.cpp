#include <iostream>
#include <string>

#include "core/TokenPlacementService.h"

using harmonies::core::placeTokenOnBoard;
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
    HexCoord neighbor(1, 0);
    HexCoord outside(5, 0);

    check(placeTokenOnBoard(board, center, TokenType::BlueWater),
          "A valid token should be placed on an empty cell",
          failures);
    check(board.getCell(center) != nullptr && board.getCell(center)->getHeight() == 1,
          "Successful placement should increase the stack height",
          failures);
    check(board.getCell(center) != nullptr && board.getCell(center)->getTokenStack().back() == TokenType::BlueWater,
          "Successful placement should put the requested token on top of the stack",
          failures);

    check(!placeTokenOnBoard(board, outside, TokenType::BlueWater),
          "Placement should fail outside the board",
          failures);

    check(placeTokenOnBoard(board, neighbor, TokenType::GrayStone),
          "A first gray stone should be placeable on an empty cell",
          failures);

    const std::size_t initialNeighborHeight = board.getCell(neighbor)->getHeight();
    check(!placeTokenOnBoard(board, neighbor, TokenType::BrownEarth),
          "An invalid stacking rule should reject the placement",
          failures);
    check(board.getCell(neighbor)->getHeight() == initialNeighborHeight,
          "A rejected placement should leave the stack height unchanged",
          failures);
    check(board.getCell(neighbor)->getTokenStack().back() == TokenType::GrayStone,
          "A rejected placement should leave the top token unchanged",
          failures);

    check(placeTokenOnBoard(board, neighbor, TokenType::GrayStone),
          "A valid second gray stone should be placeable on top of a gray stone",
          failures);
    check(board.getCell(neighbor)->getHeight() == 2,
          "A second valid placement should increase the stack height again",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
