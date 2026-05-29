#include <iostream>
#include <string>

#include "model/BoardSide.h"
#include "scoring/SoloScoreEvaluator.h"

using harmonies::model::BoardSide;
using harmonies::scoring::evaluateSoloScore;

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

    std::cout << "--- Testing SoloScoreEvaluator with Official Additive Rules ---\n";

    {
        // Test Official Manual Example: 130 points, Side A, With Spirit -> Should give 7 suns
        // Base 130 (5) + Side A (1) + Spirit (1) = 7 suns
        check(evaluateSoloScore(130, BoardSide::A, true) == 7, 
              "Official Example: 130 points, Side A, with Spirit should yield exactly 7 suns", 
              failures);
    }

    {
        // Test Peak performance: 165 points, Side A, No Spirit -> Should give 11 suns
        // Base 160+ (8) + Side A (1) + No Spirit (2) = 11 suns
        check(evaluateSoloScore(165, BoardSide::A, false) == 11, 
              "Maximum score configuration: 165 points, Side A, without Spirit should yield 11 suns", 
              failures);
    }

    {
        // Test Boundary score for base level 3 suns: 90 points, Side B, With Spirit -> Should give 4 suns
        // Base 90 (3) + Side B (0) + Spirit (1) = 4 suns
        check(evaluateSoloScore(90, BoardSide::B, true) == 4, 
              "Boundary test: 90 points, Side B, with Spirit should yield 4 suns", 
              failures);
    }

    {
        // Test Low performance boundary: 35 points, Side B, With Spirit -> Should give 1 sun
        // Base < 40 (0) + Side B (0) + Spirit (1) = 1 sun
        check(evaluateSoloScore(35, BoardSide::B, true) == 1, 
              "Low score boundary: 35 points, Side B, with Spirit should yield 1 sun", 
              failures);
    }

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}