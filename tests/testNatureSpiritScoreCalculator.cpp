#include <iostream>
#include <string>

#include "model/NatureSpiritCard.h"
#include "model/PersonalBoard.h"
#include "scoring/NatureSpiritScoreCalculator.h"

using harmonies::model::BoardSide;
using harmonies::model::NatureSpiritCard;
using harmonies::model::Pattern;
using harmonies::model::PatternCell;
using harmonies::model::PersonalBoard;
using harmonies::model::TokenType;
using harmonies::scoring::ConnectedGroupRule;
using harmonies::scoring::LandscapeCountRule;
using harmonies::scoring::NSEffectType;
using harmonies::scoring::NatureSpiritEffect;
using harmonies::scoring::natureSpiritScoreCalculator;
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
        NatureSpiritEffect effect;
        effect.type = NSEffectType::CountLandscapes;
        effect.landscapeRules.push_back(LandscapeCountRule{TokenType::GrayStone, 2, 4});

        Pattern cardPattern({{HexCoord(0, 0), TokenType::GrayStone, PatternCell::AnyHeight}});
        NatureSpiritCard card("Mountain Spirit", cardPattern, effect);
        PersonalBoard board(BoardSide::A);

        board.placeToken(HexCoord(0, 0), TokenType::GrayStone);
        board.placeToken(HexCoord(0, 0), TokenType::GrayStone);

        check(natureSpiritScoreCalculator(card, board) == 0,
              "A nature spirit card should score 0 if its cube has not been placed",
              failures);
    }

    {
        NatureSpiritEffect effect;
        effect.type = NSEffectType::CountLandscapes;
        effect.landscapeRules.push_back(LandscapeCountRule{TokenType::GrayStone, 2, 4});
        effect.landscapeRules.push_back(LandscapeCountRule{TokenType::GrayStone, 3, 10});

        Pattern cardPattern({{HexCoord(0, 0), TokenType::GrayStone, PatternCell::AnyHeight}});
        NatureSpiritCard card("Mountain Spirit", cardPattern, effect);
        PersonalBoard board(BoardSide::A);

        board.placeToken(HexCoord(0, 0), TokenType::GrayStone);
        board.placeToken(HexCoord(0, 0), TokenType::GrayStone);

        board.placeToken(HexCoord(1, 0), TokenType::GrayStone);
        board.placeToken(HexCoord(1, 0), TokenType::GrayStone);
        board.placeToken(HexCoord(1, 0), TokenType::GrayStone);

        board.placeToken(HexCoord(0, 1), TokenType::GrayStone);
        board.placeToken(HexCoord(0, 1), TokenType::GrayStone);

        card.placeCube();

        check(natureSpiritScoreCalculator(card, board) == 18,
              "Count-landscapes effects should score each matching stack",
              failures);
    }

    {
        NatureSpiritEffect effect;
        effect.type = NSEffectType::CountConnectedGroups;
        effect.connectedRules.push_back(ConnectedGroupRule{TokenType::YellowField, 1, 2, 2});
        effect.connectedRules.push_back(ConnectedGroupRule{TokenType::YellowField, 3, 0, 10});

        Pattern cardPattern({{HexCoord(0, 0), TokenType::YellowField, PatternCell::AnyHeight}});
        NatureSpiritCard card("Field Spirit", cardPattern, effect);
        PersonalBoard board(BoardSide::A);

        // First connected group of size 2.
        board.placeToken(HexCoord(0, 0), TokenType::YellowField);
        board.placeToken(HexCoord(1, 0), TokenType::YellowField);

        // Second connected group of size 3.
        board.placeToken(HexCoord(-2, -2), TokenType::YellowField);
        board.placeToken(HexCoord(-2, -1), TokenType::YellowField);
        board.placeToken(HexCoord(-1, -1), TokenType::YellowField);

        card.placeCube();

        check(natureSpiritScoreCalculator(card, board) == 12,
              "Connected-group effects should score groups according to their size range",
              failures);
    }

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
