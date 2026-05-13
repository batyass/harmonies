#include <iostream>
#include <string>

#include "model/BoardCell.h"
#include "rules/StackRule.h"

using harmonies::model::BoardCell;
using harmonies::model::StackType;
using harmonies::model::TokenType;
using harmonies::rules::StackRule;
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

    BoardCell emptyCell(HexCoord(0, 0));
    check(StackRule(emptyCell) == StackType::Empty,
          "An empty cell should be classified as Empty",
          failures);

    BoardCell earthCell(HexCoord(0, 1));
    earthCell.addToken(TokenType::BrownEarth);
    check(StackRule(earthCell) == StackType::Empty,
          "A brown earth token alone should not count as a landscape stack",
          failures);

    BoardCell treeCell(HexCoord(1, 0));
    treeCell.addToken(TokenType::BrownEarth);
    treeCell.addToken(TokenType::GreenTree);
    check(StackRule(treeCell) == StackType::Tree,
          "A green token on top of a stack should count as a tree",
          failures);

    BoardCell mountainCell(HexCoord(1, -1));
    mountainCell.addToken(TokenType::GrayStone);
    check(StackRule(mountainCell) == StackType::Mountain,
          "A gray token on top should count as a mountain",
          failures);

    BoardCell fieldCell(HexCoord(-1, 0));
    fieldCell.addToken(TokenType::YellowField);
    check(StackRule(fieldCell) == StackType::Field,
          "A yellow token on top should count as a field",
          failures);

    BoardCell waterCell(HexCoord(-1, 1));
    waterCell.addToken(TokenType::BlueWater);
    check(StackRule(waterCell) == StackType::Water,
          "A blue token on top should count as water",
          failures);

    BoardCell buildingCell(HexCoord(0, -1));
    buildingCell.addToken(TokenType::BrownEarth);
    buildingCell.addToken(TokenType::RedBuilding);
    check(StackRule(buildingCell) == StackType::Building,
          "A red token on top should count as a building",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
