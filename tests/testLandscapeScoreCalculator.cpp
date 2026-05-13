#include <iostream>
#include <string>

#include "model/PersonalBoard.h"
#include "scoring/LandscapeScoreCalculator.h"

using harmonies::model::BoardSide;
using harmonies::model::PersonalBoard;
using harmonies::model::TokenType;
using harmonies::scoring::LandscapeScoreCalculator;
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

    {
        PersonalBoard board(BoardSide::A);
        board.placeToken(HexCoord(0, 0), TokenType::BrownEarth);
        board.placeToken(HexCoord(0, 0), TokenType::BrownEarth);
        board.placeToken(HexCoord(0, 0), TokenType::GreenTree);

        check(LandscapeScoreCalculator(board) == 7,
              "A tree of height 3 should score 7 points",
              failures);
    }

    {
        PersonalBoard board(BoardSide::A);
        board.placeToken(HexCoord(0, 0), TokenType::GrayStone);

        check(LandscapeScoreCalculator(board) == 0,
              "An isolated mountain should score 0 points",
              failures);
    }

    {
        PersonalBoard board(BoardSide::A);
        board.placeToken(HexCoord(0, 0), TokenType::GrayStone);
        board.placeToken(HexCoord(1, 0), TokenType::GrayStone);
        board.placeToken(HexCoord(1, 0), TokenType::GrayStone);

        check(LandscapeScoreCalculator(board) == 4,
              "Adjacent mountains of heights 1 and 2 should score 4 points total",
              failures);
    }

    {
        PersonalBoard board(BoardSide::A);
        board.placeToken(HexCoord(0, 0), TokenType::YellowField);
        board.placeToken(HexCoord(1, 0), TokenType::YellowField);
        board.placeToken(HexCoord(1, -1), TokenType::YellowField);

        check(LandscapeScoreCalculator(board) == 5,
              "A connected field of at least 2 yellow tokens should score 5 points once",
              failures);
    }

    {
        PersonalBoard board(BoardSide::A);
        board.placeToken(HexCoord(0, 0), TokenType::BrownEarth);
        board.placeToken(HexCoord(0, 0), TokenType::RedBuilding);
        board.placeToken(HexCoord(1, 0), TokenType::YellowField);
        board.placeToken(HexCoord(1, -1), TokenType::GrayStone);
        board.placeToken(HexCoord(0, -1), TokenType::BlueWater);

        check(LandscapeScoreCalculator(board) == 5,
              "A building surrounded by at least 3 distinct neighboring colors should score 5 points",
              failures);
    }

    {
        PersonalBoard board(BoardSide::A);
        board.placeToken(HexCoord(0, 0), TokenType::BlueWater);
        board.placeToken(HexCoord(1, 0), TokenType::BlueWater);
        board.placeToken(HexCoord(2, 0), TokenType::BlueWater);

        check(LandscapeScoreCalculator(board) == 5,
              "On side A, a river of length 3 should score 5 points",
              failures);
    }

    {
        PersonalBoard board(BoardSide::B);

        check(LandscapeScoreCalculator(board) == 5,
              "On side B, an empty board should count as one island worth 5 points",
              failures);
    }

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
