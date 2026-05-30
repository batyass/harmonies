#include <iostream>
#include <string>
#include <vector>

#include "ui/ConsoleRenderer.h"
#include "model/CentralBoard.h"
#include "model/PersonalBoard.h"
#include "model/Player.h"
#include "model/ScoreReport.h"
#include "model/TokenType.h"
#include "utils/hexCoord.h"

using harmonies::model::BoardSide;
using harmonies::model::CentralBoard;
using harmonies::model::PersonalBoard;
using harmonies::model::Player;
using harmonies::model::ScoreReport;
using harmonies::model::TokenType;
using harmonies::ui::ConsoleRenderer;
using harmonies::utils::HexCoord;

namespace
{
    void check(bool condition, const std::string &message, int &failures)
    {
        if (condition)
            std::cout << "[PASS] " << message << '\n';
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
    ConsoleRenderer renderer;


    std::cout << "\n--- displayCentralBoard ---\n";
    {
        CentralBoard central(2);
        central.getSlot(0)->fill({TokenType::BlueWater, TokenType::GreenTree, TokenType::BrownEarth});
        central.getSlot(2)->fill({TokenType::GrayStone, TokenType::GrayStone, TokenType::GrayStone});
        central.getSlot(3)->fill({TokenType::YellowField, TokenType::RedBuilding, TokenType::BlueWater});
        central.getSlot(4)->fill({TokenType::BlueWater, TokenType::GreenTree, TokenType::BrownEarth});

        renderer.displayCentralBoard(central);

        check(central.getNbSlots() == 5,
              "2-player CentralBoard has 5 slots",
              failures);
        check(!central.getSlot(0)->isEmpty(),
              "Slot 0 is not empty after fill",
              failures);
        check(central.getSlot(1)->isEmpty(),
              "Slot 1 is empty (before fill)",
              failures);
    }

   
    std::cout << "\n--- displayPersonalBoard (Side A) ---\n";
    {
        PersonalBoard board(BoardSide::A);

        board.placeToken(HexCoord(0, 0), TokenType::BlueWater);

        board.placeToken(HexCoord(-1, 0), TokenType::BrownEarth);
        board.placeToken(HexCoord(-1, 0), TokenType::GreenTree);

        board.placeToken(HexCoord(1, 0), TokenType::GrayStone);
        board.placeToken(HexCoord(1, 0), TokenType::GrayStone);
        board.placeToken(HexCoord(1, 0), TokenType::GrayStone);

        board.placeToken(HexCoord(-2, 0), TokenType::BlueWater);
        board.getCell(HexCoord(-2, 0))->placeCube();

        board.placeToken(HexCoord(2, 0), TokenType::BrownEarth);
        board.placeToken(HexCoord(2, 0), TokenType::GreenTree);
        board.getCell(HexCoord(2, 0))->placeCube();

        board.placeToken(HexCoord(0, -2), TokenType::YellowField);
        board.placeToken(HexCoord(0,  2), TokenType::RedBuilding);

        renderer.displayPersonalBoard(board);

        check(board.getCell(HexCoord(1, 0))->getHeight() == 3,
              "Triple-stacked cell has height 3",
              failures);
        check(board.getCell(HexCoord(-2, 0))->hasCube(),
              "Cell (-2,0) has a cube after placeCube()",
              failures);
        check(!board.getCell(HexCoord(2, 0))->canPlaceCube(),
              "Cell (2,0) cannot receive a second cube",
              failures);
    }


    std::cout << "\n--- displayScoreReport ---\n";
    {
        ScoreReport report;
        report.addTreeScore(5);
        report.addMountainScore(8);
        report.addFieldScore(3);
        report.addBuildingScore(4);
        report.addWaterScore(6);
        report.addAnimalsScore(7);
        report.addSpiritScore(2);

        renderer.displayScoreReport(report, "Alice");

        check(report.getTotalScore() == 35,
              "getTotalScore() returns 5+8+3+4+6+7+2 = 35",
              failures);
    }

    
    std::cout << "\n--- displayEndGame ---\n";
    {
        Player alice("Alice",   BoardSide::A);
        Player bob("Bob",     BoardSide::A);
        Player charlie("Charlie", BoardSide::A);

        alice.addPoints(35);
        bob.addPoints(28);
        charlie.addPoints(42);

        std::vector<Player *> players = {&alice, &bob, &charlie};
        renderer.displayEndGame(players);


        check(players[0]->getName() == "Alice",
              "displayEndGame does not reorder the caller's vector",
              failures);
        check(charlie.getScore() == 42,
              "Charlie has 42 pts (highest)",
              failures);
    }

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
