#include <iostream>
#include <string>

#include "rules/PlacementValidator.h"

using harmonies::model::BoardSide;
using harmonies::model::PersonalBoard;
using harmonies::model::TokenType;
using harmonies::rules::placementValidator;
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

    check(placementValidator(TokenType::BlueWater, center, board),
          "Any token should currently be placeable on an empty valid cell",
          failures);
    check(!placementValidator(TokenType::BlueWater, outside, board),
          "A token should not be placeable outside the board",
          failures);

    board.getCell(center)->placeCube();
    check(!placementValidator(TokenType::GrayStone, center, board),
          "No token should be placeable on a cell containing a cube",
          failures);

    board.placeToken(neighbor, TokenType::GrayStone);
    check(placementValidator(TokenType::GrayStone, neighbor, board),
          "Gray stone should be placeable on top of gray stone",
          failures);
    check(placementValidator(TokenType::RedBuilding, neighbor, board),
          "Red building should be placeable on a single gray stone",
          failures);
    check(!placementValidator(TokenType::BrownEarth, neighbor, board),
          "Brown earth should not be placeable on a single gray stone",
          failures);

    HexCoord earthCell(0, 1);
    board.placeToken(earthCell, TokenType::BrownEarth);
    check(placementValidator(TokenType::GreenTree, earthCell, board),
          "Green tree should be placeable on top of brown earth",
          failures);
    check(placementValidator(TokenType::RedBuilding, earthCell, board),
          "Red building should be placeable on a single brown earth token",
          failures);

    HexCoord buildingStack(1, -1);
    board.placeToken(buildingStack, TokenType::GrayStone);
    board.placeToken(buildingStack, TokenType::GrayStone);
    check(!placementValidator(TokenType::RedBuilding, buildingStack, board),
          "Red building should not be placeable on a stack of height 2",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
