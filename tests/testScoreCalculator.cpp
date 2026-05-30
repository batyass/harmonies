#include <iostream>
#include <string>

#include "model/Player.h"
#include "model/BoardSide.h"
#include "model/ScoreReport.h"
#include "scoring/ScoreCalculator.h"

using harmonies::model::BoardSide;
using harmonies::model::Player;
using harmonies::model::ScoreReport;
using harmonies::scoring::calculateTotalScore;

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

    std::cout << "--- Testing ScoreCalculator ---\n";

    {
        Player player("Peilin", BoardSide::A);
        ScoreReport report = calculateTotalScore(player);

        check(report.getTreeScore() == 0,
              "An empty player assets calculation should return 0 tree score",
              failures);
        check(report.getAnimalsScore() == 0,
              "An empty player assets calculation should return 0 animals score",
              failures);
        check(report.getSpiritScore() == 0,
              "An empty player assets calculation should return 0 spirit score",
              failures);
        check(report.getTotalScore() == 0,
              "An empty player board should sum up to exactly 0 total points",
              failures);
    }

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}