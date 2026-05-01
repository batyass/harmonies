#include <iostream>
#include <string>

#include "model/ScoreReport.h"

using harmonies::model::ScoreReport;

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

    ScoreReport report;

    check(report.getTotalScore() == 0,
          "A new score report should start at 0",
          failures);

    report.addTreeScore(3);
    report.addMountainScore(4);
    report.addFieldScore(5);
    report.addBuildingScore(2);
    report.addWaterScore(1);
    report.addAnimalsScore(6);
    report.addSpiritScore(7);

    check(report.getTreeScore() == 3,
          "The tree score should be accumulated correctly",
          failures);
    check(report.getMountainScore() == 4,
          "The mountain score should be accumulated correctly",
          failures);
    check(report.getFieldScore() == 5,
          "The field score should be accumulated correctly",
          failures);
    check(report.getBuildingScore() == 2,
          "The building score should be accumulated correctly",
          failures);
    check(report.getWaterScore() == 1,
          "The water score should be accumulated correctly",
          failures);
    check(report.getAnimalsScore() == 6,
          "The animal score should be accumulated correctly",
          failures);
    check(report.getSpiritScore() == 7,
          "The spirit score should be accumulated correctly",
          failures);
    check(report.getTotalScore() == 28,
          "The total score should sum all detailed categories",
          failures);

    report.resetScore();
    check(report.getTotalScore() == 0,
          "Resetting a score report should clear every category",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
