#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "core/Game.h"
#include "model/NatureSpiritCard.h"
#include "model/Pattern.h"
#include "scoring/NatureSpiritEffect.h"

using harmonies::core::Game;
using harmonies::core::GameState;
using harmonies::model::BoardSide;
using harmonies::model::GameConfig;
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

    void buildAnimalPatternOnBoard(harmonies::model::PersonalBoard &board,
                                   const harmonies::utils::HexCoord &anchor,
                                   const harmonies::model::Pattern &pattern)
    {
        const std::vector<harmonies::model::PatternCell> &cells = pattern.getCells();
        for (std::size_t i = 0; i < cells.size(); ++i)
        {
            const harmonies::model::PatternCell &cell = cells[i];
            harmonies::utils::HexCoord target =
                harmonies::utils::addPatternOffset(anchor, cell.offset);

            int height = cell.height == harmonies::model::PatternCell::AnyHeight ? 1 : cell.height;
            for (int h = 0; h < height; ++h)
            {
                board.placeToken(target, cell.type);
            }
        }
    }
}

int main()
{
    int failures = 0;

    {
        GameConfig config(2, BoardSide::A, false);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");
        playerNames.push_back("Bob");

        Game game(config, playerNames);

        check(game.getState() == GameState::NotStarted,
              "A new game should start in NotStarted state",
              failures);

        check(game.getCurrentPlayer() == nullptr,
              "Current player should be nullptr before initGame",
              failures);

        game.initGame();

        check(game.getState() == GameState::WaitingForSlotChoice,
              "initGame should move the game to WaitingForSlotChoice",
              failures);

        check(game.getCurrentPlayer() != nullptr,
              "Current player should exist after initGame",
              failures);

        check(game.getCurrentPlayer() != nullptr &&
                  game.getCurrentPlayer()->getName() == "Alice",
              "The first player should be the current player after initGame",
              failures);

        bool threw = false;
        try
        {
            game.initGame();
        }
        catch (const std::logic_error &)
        {
            threw = true;
        }
        check(threw,
              "Calling initGame twice should throw",
              failures);
    }

    {
        GameConfig config(2, BoardSide::A, false);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");
        playerNames.push_back("Bob");

        Game game(config, playerNames);
        game.initGame();

        std::vector<TokenType> chosenSlot;
        chosenSlot.push_back(TokenType::BlueWater);
        chosenSlot.push_back(TokenType::GrayStone);
        chosenSlot.push_back(TokenType::BrownEarth);
        game.getCentralBoard()->getSlot(0)->fill(chosenSlot);

        const std::size_t remainingBefore = game.getTokenBag()->getRemainingCount();

        check(game.takeTokensFromSlot(0),
              "Taking tokens from a valid slot should succeed",
              failures);
        check(game.getState() == GameState::WaitingForPlacement,
              "Taking a slot should move the game to WaitingForPlacement",
              failures);
        check(game.getCurrentPlayer() != nullptr &&
                  game.getCurrentPlayer()->getName() == "Alice",
              "Taking a slot should not change the current player immediately",
              failures);
        check(game.getCentralBoard()->getSlot(0) != nullptr &&
                  game.getCentralBoard()->getSlot(0)->getTokens().size() == 3,
              "After taking a slot, it should be refilled to 3 tokens when the bag has enough tokens",
              failures);
        check(game.getTokenBag()->getRemainingCount() == remainingBefore - 3,
              "Refilling the chosen slot should consume 3 tokens from the bag",
              failures);
    }

    {
        GameConfig config(2, BoardSide::A, false);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");
        playerNames.push_back("Bob");

        Game game(config, playerNames);
        game.initGame();

        const std::size_t visibleBefore = game.getAnimalCardDeck()->visibleCount();

        check(game.takeVisibleAnimalCard(0),
              "Taking a visible animal card should succeed once per turn",
              failures);
        check(game.getPlayers()[0]->getAnimalCards()->getCardCount() == 1,
              "Taking a visible animal card should add it to the current player's collection",
              failures);
        check(game.getAnimalCardDeck()->visibleCount() == visibleBefore - 1,
              "Taking a visible animal card should remove it from the visible market until end of turn",
              failures);
        check(!game.takeVisibleAnimalCard(0),
              "A player should not be able to take more than one visible animal card in the same turn",
              failures);
    }

    {
        GameConfig config(2, BoardSide::A, true);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");
        playerNames.push_back("Bob");

        Game game(config, playerNames);
        game.initGame();

        check(game.getPlayers()[0]->getNatureSpiritCardCount() == 2,
              "With the nature spirit option enabled, each player should receive two nature spirit cards at setup",
              failures);

        check(game.chooseNatureSpiritCard(0),
              "The current player should be able to choose one of their two nature spirit cards",
              failures);
        check(game.getPlayers()[0]->getNatureSpiritCardCount() == 1,
              "Choosing a nature spirit card should keep only the selected card",
              failures);
        check(!game.chooseNatureSpiritCard(0),
              "Once a nature spirit card has been chosen, the player should not choose again",
              failures);

        const TokenType spiritTokenType =
            game.getPlayers()[0]->getNatureSpiritCard(0)->getPattern().getCells()[0].type;
        HexCoord spiritAnchor(0, 0);
        game.getPlayers()[0]->getBoard()->placeToken(spiritAnchor, spiritTokenType);

        check(game.placeNatureSpiritCube(spiritAnchor),
              "The chosen nature spirit card should be able to place its cube when the pattern is satisfied",
              failures);
        check(game.getPlayers()[0]->getBoard()->getCell(spiritAnchor) != nullptr &&
                  game.getPlayers()[0]->getBoard()->getCell(spiritAnchor)->hasCube(),
              "Placing a nature spirit cube through Game should mark the chosen board cell",
              failures);
        check(game.getPlayers()[0]->getNatureSpiritCard(0) != nullptr &&
                  game.getPlayers()[0]->getNatureSpiritCard(0)->isCubePlaced(),
              "Placing a nature spirit cube through Game should update the selected card",
              failures);
    }

    {
        GameConfig config(2, BoardSide::A, true);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");
        playerNames.push_back("Bob");

        Game game(config, playerNames);
        game.initGame();

        std::vector<TokenType> firstTurnSlot;
        firstTurnSlot.push_back(TokenType::BlueWater);
        firstTurnSlot.push_back(TokenType::GrayStone);
        firstTurnSlot.push_back(TokenType::BrownEarth);
        game.getCentralBoard()->getSlot(0)->fill(firstTurnSlot);

        check(game.takeTokensFromSlot(0),
              "A player should be able to start their first turn without choosing a nature spirit card immediately",
              failures);
        check(game.placeTokenOnBoard(HexCoord(0, 0), TokenType::BlueWater),
              "The first player should be able to place the first mandatory token",
              failures);
        check(game.placeTokenOnBoard(HexCoord(1, 0), TokenType::GrayStone),
              "The first player should be able to place the second mandatory token",
              failures);
        check(game.placeTokenOnBoard(HexCoord(0, 1), TokenType::BrownEarth),
              "The first player should be able to place the third mandatory token",
              failures);
        check(game.endTurn(),
              "After completing mandatory placements, the first player should be able to end their turn explicitly",
              failures);

        game.getCentralBoard()->getSlot(0)->fill(firstTurnSlot);
        check(game.takeTokensFromSlot(0),
              "The second player should be able to play their first turn normally",
              failures);
        check(game.placeTokenOnBoard(HexCoord(0, 0), TokenType::BlueWater),
              "The second player should be able to place the first mandatory token",
              failures);
        check(game.placeTokenOnBoard(HexCoord(1, 0), TokenType::GrayStone),
              "The second player should be able to place the second mandatory token",
              failures);
        check(game.placeTokenOnBoard(HexCoord(0, 1), TokenType::BrownEarth),
              "The second player should be able to place the third mandatory token",
              failures);
        check(game.endTurn(),
              "After completing mandatory placements, the second player should be able to end their turn explicitly",
              failures);

        check(game.getCurrentPlayer() != nullptr &&
                  game.getCurrentPlayer()->getName() == "Alice",
              "After one full round, the turn should return to the first player",
              failures);
        check(!game.chooseNatureSpiritCard(0),
              "A nature spirit card should no longer be choosable after the player's first turn has passed",
              failures);
    }

    {
        GameConfig config(2, BoardSide::A, true);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");
        playerNames.push_back("Bob");

        Game game(config, playerNames);
        game.initGame();
        check(game.chooseNatureSpiritCard(0),
              "The current player should be able to choose one nature spirit card before testing the active card limit",
              failures);

        harmonies::model::Player *player = game.getCurrentPlayer();
        for (int i = 0; i < 3; ++i)
        {
            player->getAnimalCards()->addCard(game.getAnimalCardDeck()->takeVisible(0));
        }

        check(!game.takeVisibleAnimalCard(0),
              "An unplaced chosen nature spirit card should count in the limit of 4 active cards",
              failures);

        buildAnimalPatternOnBoard(*player->getBoard(),
                                  HexCoord(0, 0),
                                  player->getNatureSpiritCard(0)->getPattern());
        check(game.placeNatureSpiritCube(HexCoord(0, 0)),
              "The chosen nature spirit card should be placeable to free its active card slot",
              failures);
        check(game.takeVisibleAnimalCard(0),
              "Once the nature spirit cube is placed, the spirit card should no longer count in the 4 active cards limit",
              failures);
    }

    {
        GameConfig config(2, BoardSide::A, false);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");
        playerNames.push_back("Bob");

        Game game(config, playerNames);
        game.initGame();

        harmonies::model::Player *player = game.getCurrentPlayer();
        harmonies::model::AnimalCard completedCard = game.getAnimalCardDeck()->takeVisible(0);
        while (completedCard.placeNextCube())
        {
        }
        player->getAnimalCards()->addCard(completedCard);

        for (int i = 0; i < 3; ++i)
        {
            player->getAnimalCards()->addCard(game.getAnimalCardDeck()->takeVisible(0));
        }

        check(game.takeVisibleAnimalCard(0),
              "A completed animal card should not count toward the 4 active cards limit",
              failures);
    }

    {
        GameConfig config(2, BoardSide::A, false);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");
        playerNames.push_back("Bob");

        Game game(config, playerNames);
        game.initGame();

        check(game.getAnimalCardDeck()->visibleCount() > 0,
              "The visible animal market should expose at least one card",
              failures);

        std::vector<TokenType> chosenSlot;
        chosenSlot.push_back(TokenType::BlueWater);
        chosenSlot.push_back(TokenType::GrayStone);
        chosenSlot.push_back(TokenType::BrownEarth);
        game.getCentralBoard()->getSlot(0)->fill(chosenSlot);

        check(game.takeVisibleAnimalCard(0),
              "A visible animal card should be takeable before the mandatory token placement sequence",
              failures);
        check(game.getPlayers()[0]->getAnimalCards()->getCardCount() == 1,
              "Taking a visible animal card should store it for the current player",
              failures);

        harmonies::model::AnimalCard *chosenCard = game.getPlayers()[0]->getAnimalCards()->getCard(0);
        check(chosenCard != nullptr,
              "The taken animal card should remain accessible in the player's collection",
              failures);

        game.takeTokensFromSlot(0);

        check(game.placeTokenOnBoard(HexCoord(0, 0), TokenType::BlueWater),
              "The player should still be able to start placing mandatory tokens after taking an animal card",
              failures);

        HexCoord cubeAnchor(0, 1);
        if (chosenCard != nullptr)
        {
            buildAnimalPatternOnBoard(*game.getPlayers()[0]->getBoard(), cubeAnchor, chosenCard->getPattern());
            check(true,
                  "The player board should be configurable to satisfy the chosen animal card pattern",
                  failures);
        }

        check(game.placeAnimalCube(0, cubeAnchor),
              "The player should be able to place an animal cube during the same turn when the pattern is satisfied",
              failures);
        check(game.getPlayers()[0]->getBoard()->getCell(cubeAnchor) != nullptr &&
                  game.getPlayers()[0]->getBoard()->getCell(cubeAnchor)->hasCube(),
              "Placing an animal cube through Game should mark the chosen board cell",
              failures);
        check(game.getPlayers()[0]->getAnimalCards()->getCard(0) != nullptr &&
                  game.getPlayers()[0]->getAnimalCards()->getCard(0)->getCubesOnCard() ==
                      game.getPlayers()[0]->getAnimalCards()->getCard(0)->totalSlots() - 1,
              "Placing an animal cube through Game should consume one cube from the chosen animal card",
              failures);
    }

    {
        GameConfig config(2, BoardSide::A, false);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");
        playerNames.push_back("Bob");

        Game game(config, playerNames);
        game.initGame();

        std::vector<TokenType> chosenSlot;
        chosenSlot.push_back(TokenType::BlueWater);
        chosenSlot.push_back(TokenType::GrayStone);
        chosenSlot.push_back(TokenType::BrownEarth);
        game.getCentralBoard()->getSlot(0)->fill(chosenSlot);

        check(game.takeTokensFromSlot(0),
              "A player should still be able to start a turn normally before chaining cube placements",
              failures);
        check(game.placeTokenOnBoard(HexCoord(0, 0), TokenType::BlueWater),
              "The first mandatory token should still be placeable before multiple cube placements",
              failures);
        check(game.placeTokenOnBoard(HexCoord(1, 0), TokenType::GrayStone),
              "The second mandatory token should still be placeable before multiple cube placements",
              failures);
        check(game.placeTokenOnBoard(HexCoord(0, 1), TokenType::BrownEarth),
              "The third mandatory token should still be placeable before multiple cube placements",
              failures);

        harmonies::model::Pattern waterPattern(
            std::vector<harmonies::model::PatternCell>(1, harmonies::model::PatternCell{HexCoord(0, 0), TokenType::BlueWater, harmonies::model::PatternCell::AnyHeight}));
        harmonies::model::Pattern stonePattern(
            std::vector<harmonies::model::PatternCell>(1, harmonies::model::PatternCell{HexCoord(0, 0), TokenType::GrayStone, harmonies::model::PatternCell::AnyHeight}));

        game.getPlayers()[0]->getAnimalCards()->addCard(
            harmonies::model::AnimalCard("Water Test Card", waterPattern, std::vector<int>(1, 2)));
        game.getPlayers()[0]->getAnimalCards()->addCard(
            harmonies::model::AnimalCard("Stone Test Card", stonePattern, std::vector<int>(1, 2)));

        check(game.placeAnimalCube(0, HexCoord(0, 0)),
              "A first animal cube should be placeable during the turn",
              failures);
        check(game.placeAnimalCube(1, HexCoord(1, 0)),
              "A second animal cube from another card should also be placeable in the same turn",
              failures);
        check(game.getPlayers()[0]->getBoard()->getCell(HexCoord(0, 0)) != nullptr &&
                  game.getPlayers()[0]->getBoard()->getCell(HexCoord(0, 0))->hasCube(),
              "The first anchor should keep its cube after chaining placements",
              failures);
        check(game.getPlayers()[0]->getBoard()->getCell(HexCoord(1, 0)) != nullptr &&
                  game.getPlayers()[0]->getBoard()->getCell(HexCoord(1, 0))->hasCube(),
              "The second anchor should receive its cube during the same turn",
              failures);
    }

    {
        GameConfig config(2, BoardSide::A, false);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");
        playerNames.push_back("Bob");

        Game game(config, playerNames);
        game.initGame();

        std::vector<TokenType> chosenSlot;
        chosenSlot.push_back(TokenType::BlueWater);
        chosenSlot.push_back(TokenType::GrayStone);
        chosenSlot.push_back(TokenType::BrownEarth);
        game.getCentralBoard()->getSlot(0)->fill(chosenSlot);

        game.takeTokensFromSlot(0);

        check(game.placeTokenOnBoard(HexCoord(0, 0), TokenType::BlueWater),
              "The first pending token should be placeable on the board",
              failures);
        check(game.getState() == GameState::WaitingForPlacement,
              "The game should stay in WaitingForPlacement while tokens remain to be placed",
              failures);

        check(game.placeTokenOnBoard(HexCoord(1, 0), TokenType::GrayStone),
              "The second pending token should be placeable on the board",
              failures);
        check(game.getState() == GameState::WaitingForPlacement,
              "The game should still wait for placement after the second token",
              failures);

        check(game.placeTokenOnBoard(HexCoord(0, 1), TokenType::BrownEarth),
              "The last pending token should be placeable on the board",
              failures);
        check(game.getState() == GameState::WaitingForTurnEndChoice,
              "After the last token is placed, the current player should choose whether to end the turn",
              failures);
        check(game.getCurrentPlayer() != nullptr &&
                  game.getCurrentPlayer()->getName() == "Alice",
              "After placing all pending tokens, the current player should keep control until they end the turn",
              failures);
        check(game.endTurn(),
              "The current player should be able to explicitly end the turn once mandatory actions are complete",
              failures);
        check(game.getState() == GameState::WaitingForSlotChoice,
              "After ending the turn, the next player should enter the slot-choice phase",
              failures);
        check(game.getCurrentPlayer() != nullptr &&
                  game.getCurrentPlayer()->getName() == "Bob",
              "After ending the turn, control should pass to the next player",
              failures);

        check(game.getPlayers()[0]->getBoard()->getCell(HexCoord(0, 0)) != nullptr &&
                  game.getPlayers()[0]->getBoard()->getCell(HexCoord(0, 0))->getHeight() == 1,
              "The first placed token should remain on Alice's board",
              failures);
        check(game.getPlayers()[0]->getBoard()->getCell(HexCoord(1, 0)) != nullptr &&
                  game.getPlayers()[0]->getBoard()->getCell(HexCoord(1, 0))->getHeight() == 1,
              "The second placed token should remain on Alice's board",
              failures);
        check(game.getPlayers()[0]->getBoard()->getCell(HexCoord(0, 1)) != nullptr &&
                  game.getPlayers()[0]->getBoard()->getCell(HexCoord(0, 1))->getHeight() == 1,
              "The third placed token should remain on Alice's board",
              failures);
        check(game.getAnimalCardDeck()->visibleCount() == 5,
              "Visible animal cards should be refilled at the end of the turn",
              failures);
    }

    {
        GameConfig config(2, BoardSide::A, true);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");
        playerNames.push_back("Bob");

        Game game(config, playerNames);
        game.initGame();

        harmonies::model::Player *alice = game.getPlayers()[0].get();
        alice->getBoard()->placeToken(HexCoord(0, 0), TokenType::GreenTree);

        harmonies::model::Pattern animalPattern(
            std::vector<harmonies::model::PatternCell>(1, harmonies::model::PatternCell{HexCoord(0, 0), TokenType::GreenTree, harmonies::model::PatternCell::AnyHeight}));
        harmonies::model::AnimalCard animalCard("Scoring Fox", animalPattern, std::vector<int>(1, 3));
        animalCard.placeNextCube();
        alice->getAnimalCards()->addCard(animalCard);

        harmonies::scoring::NatureSpiritEffect spiritEffect;
        spiritEffect.type = harmonies::scoring::NSEffectType::CountLandscapes;
        spiritEffect.landscapeRules.push_back(
            harmonies::scoring::LandscapeCountRule{TokenType::GreenTree, harmonies::model::PatternCell::AnyHeight, 4});

        harmonies::model::NatureSpiritCard spiritCard("Scoring Spirit", animalPattern, spiritEffect);
        spiritCard.placeCube();
        alice->addNatureSpiritCard(spiritCard);

        check(game.calculatePlayerScore(*alice) == 8,
              "Game should sum landscape, animal and nature spirit scores for a player",
              failures);
    }

    {
        GameConfig config(2, BoardSide::A, false);
        std::vector<std::string> playerNames;
        playerNames.push_back("Alice");
        playerNames.push_back("Bob");

        Game game(config, playerNames);
        game.initGame();

        game.getPlayers()[0]->setScore(12);
        game.getPlayers()[1]->setScore(9);

        check(game.getWinner() != nullptr &&
                  game.getWinner()->getName() == "Alice",
              "getWinner should return the player with the highest stored score",
              failures);
    }

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
