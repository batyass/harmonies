#include <iostream>
#include <string>
#include <vector>

#include "core/AnimalCubePlacementService.h"
#include "model/AnimalCard.h"
#include "model/PersonalBoard.h"

using harmonies::core::placeAnimalCube;
using harmonies::model::AnimalCard;
using harmonies::model::BoardSide;
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

    Pattern pattern({{HexCoord(0, 0), TokenType::GrayStone, PatternCell::AnyHeight}});
    HexCoord anchor(0, 0);

    {
        PersonalBoard board(BoardSide::A);
        AnimalCard card("Wolf", pattern, {3, 5});

        check(!placeAnimalCube(card, board, anchor),
              "placeAnimalCube should fail on an empty board (pattern mismatch)",
              failures);
        check(card.getCubesOnCard() == 2,
              "No cube should be removed from the card after a failed placement",
              failures);
    }

    {
        PersonalBoard board(BoardSide::A);
        AnimalCard card("Wolf", pattern, {3, 5});

        board.placeToken(anchor, TokenType::GrayStone);

        check(placeAnimalCube(card, board, anchor),
              "placeAnimalCube should succeed when the pattern is satisfied",
              failures);
        check(card.getCubesOnCard() == 1,
              "One cube should be removed from the card after a successful placement",
              failures);
        check(board.getCell(anchor)->hasCube(),
              "The anchor cell should have a cube after placement",
              failures);
    }

    {
        PersonalBoard board(BoardSide::A);
        AnimalCard card("Wolf", pattern, {3, 5});

        board.placeToken(anchor, TokenType::GrayStone);
        board.getCell(anchor)->placeCube(); // cube deja present

        check(!placeAnimalCube(card, board, anchor),
              "placeAnimalCube should fail when the cell already has a cube",
              failures);
        check(card.getCubesOnCard() == 2,
              "No cube should be removed from the card when the cell is blocked",
              failures);
    }

    {
        PersonalBoard board(BoardSide::A);
        AnimalCard card("Wolf", pattern, {3});

        card.placeNextCube();

        board.placeToken(anchor, TokenType::GrayStone);

        check(!placeAnimalCube(card, board, anchor),
              "placeAnimalCube should fail when the card is already complete",
              failures);
    }

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
