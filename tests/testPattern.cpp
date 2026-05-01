#include <iostream>
#include <string>
#include <vector>

#include "model/Pattern.h"

using harmonies::model::Pattern;
using harmonies::model::PatternCell;
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

    std::vector<PatternCell> cells = {
        {HexCoord(0, 0), TokenType::BlueWater, PatternCell::AnyHeight},
        {HexCoord(1, 0), TokenType::GreenTree, 2}};

    Pattern pattern(cells);
    Pattern rotatedOnce = pattern.rotated(1);
    Pattern rotatedSixTimes = pattern.rotated(6);

    check(pattern.getCells().size() == 2,
          "A pattern should expose all of its cells",
          failures);
    check(rotatedOnce.getCells()[1].offset == HexCoord(1, -1),
          "A clockwise rotation should update hex offsets correctly",
          failures);
    check(rotatedOnce.getCells()[1].type == TokenType::GreenTree,
          "A rotation should preserve the token type",
          failures);
    check(rotatedOnce.getCells()[1].height == 2,
          "A rotation should preserve the required height",
          failures);
    check(rotatedSixTimes.getCells()[1].offset == pattern.getCells()[1].offset,
          "Rotating six times should return to the original position",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
