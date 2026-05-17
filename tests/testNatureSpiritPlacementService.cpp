#include <iostream>
#include <string>
#include <vector>

#include "core/NatureSpiritPlacementService.h"
#include "model/NatureSpiritCard.h"
#include "model/PersonalBoard.h"

using harmonies::core::placeNatureSpiritCube;
using harmonies::model::BoardSide;
using harmonies::model::NatureSpiritCard;
using harmonies::model::Pattern;
using harmonies::model::PatternCell;
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

    Pattern pattern({{HexCoord(0, 0), TokenType::BlueWater, PatternCell::AnyHeight}});
    HexCoord anchor(0, 0);

    {
        PersonalBoard board(BoardSide::A);
        NatureSpiritCard card("Brook Spirit", pattern, nullptr);

        check(!placeNatureSpiritCube(card, board, anchor),
              "placeNatureSpiritCube should fail on an empty board (pattern mismatch)",
              failures);
        check(!card.isCubePlaced(),
              "Cube should still be on the card after a failed placement",
              failures);
    }

    {
        PersonalBoard board(BoardSide::A);
        NatureSpiritCard card("Brook Spirit", pattern, nullptr);

        board.placeToken(anchor, TokenType::BlueWater);

        check(placeNatureSpiritCube(card, board, anchor),
              "placeNatureSpiritCube should succeed when the pattern is satisfied",
              failures);
        check(card.isCubePlaced(),
              "The card should report its cube as placed after a successful placement",
              failures);
        check(board.getCell(anchor)->hasCube(),
              "The anchor cell should have a cube after placement",
              failures);
    }

    {
        PersonalBoard board(BoardSide::A);
        NatureSpiritCard card("Brook Spirit", pattern, nullptr);

        card.placeCube(); // cube deja pose manuellement

        board.placeToken(anchor, TokenType::BlueWater);

        check(!placeNatureSpiritCube(card, board, anchor),
              "placeNatureSpiritCube should fail when the cube is already placed",
              failures);
    }

    {
        PersonalBoard board(BoardSide::A);
        NatureSpiritCard card("Brook Spirit", pattern, nullptr);

        board.placeToken(anchor, TokenType::BlueWater);
        board.getCell(anchor)->placeCube(); // cube deja present sur la case

        check(!placeNatureSpiritCube(card, board, anchor),
              "placeNatureSpiritCube should fail when the cell already has a cube",
              failures);
        check(!card.isCubePlaced(),
              "Cube should still be on the card when the cell is blocked",
              failures);
    }

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
