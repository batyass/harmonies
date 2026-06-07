#include <iostream>
#include <vector>
#include <string>

#include "rules/PatternMatcher.h"
#include "model/PersonalBoard.h"
#include "model/TokenType.h"
#include "utils/hexCoord.h"

using harmonies::model::PersonalBoard;
using harmonies::model::BoardSide;
using harmonies::model::TokenType;
using harmonies::model::Pattern;
using harmonies::model::PatternCell;
using harmonies::utils::HexCoord;
using harmonies::rules::matchesAnyRotation;

namespace
{
    /**
     * Helper function to verify test conditions and track failures.
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
    // Initialize the board with Side A
    PersonalBoard board(BoardSide::A);

    /**
     * Setup: Create a simple pattern.
     * Required: GrayStone at (0,0) and BlueWater at (1,0), both at height 1.
     */
    std::vector<PatternCell> cells = {
        {HexCoord(0, 0), TokenType::GrayStone, 1},
        {HexCoord(1, 0), TokenType::BlueWater, 1}
    };
    Pattern myPattern(cells);

    // Coordinate definitions for testing
    HexCoord anchor(0, 0);
    HexCoord pos10(1, 0); // Original target position
    // Test 1: Verify that an empty board does not match the pattern
    check(!matchesAnyRotation(board, anchor, myPattern),
          "Should not match on an empty board", failures);

    // Test 2: Standard matching logic
    // Place tokens manually to satisfy the pattern requirements
    board.placeToken(anchor, TokenType::GrayStone);
    board.placeToken(pos10, TokenType::BlueWater);
    check(matchesAnyRotation(board, anchor, myPattern),
          "Should match when tokens are correctly placed", failures);

    // Test 3: Rotational matching logic
    /**
     * To test rotation without a clear/reset function in BoardCell,
     * we place a token at the geometrically rotated target relative to a new anchor.
     */
    HexCoord newAnchor(-1, 1);
    HexCoord rotatedTarget = harmonies::utils::addPatternOffset(newAnchor, HexCoord(1, 1));
    
    board.placeToken(newAnchor, TokenType::GrayStone);
    board.placeToken(rotatedTarget, TokenType::BlueWater);

    check(matchesAnyRotation(board, newAnchor, myPattern),
          "MatchesAnyRotation should succeed on a rotated setup", failures);

    // Test 4: Matching must also work when the anchor is on an odd column.
    Pattern openCornerPattern({
        {HexCoord(0, 0), TokenType::YellowField, 1},
        {HexCoord(1, 0), TokenType::RedBuilding, 2},
        {HexCoord(0, 1), TokenType::RedBuilding, 2}});

    HexCoord oddAnchor(-1, -1);
    HexCoord firstBuilding = harmonies::utils::addPatternOffset(oddAnchor, HexCoord(1, 0));
    HexCoord secondBuilding = harmonies::utils::addPatternOffset(oddAnchor, HexCoord(0, 1));

    board.placeToken(oddAnchor, TokenType::YellowField);
    board.placeToken(firstBuilding, TokenType::BrownEarth);
    board.placeToken(firstBuilding, TokenType::RedBuilding);
    board.placeToken(secondBuilding, TokenType::BrownEarth);
    board.placeToken(secondBuilding, TokenType::RedBuilding);

    check(matchesAnyRotation(board, oddAnchor, openCornerPattern),
          "A pattern should translate correctly even when anchored on an odd column",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
