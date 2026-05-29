#include "core/Game.h"
#include "rules/EndGameChecker.h"
#include "core/SlotSelectionService.h"
#include "core/TokenPlacementService.h"
#include <stdexcept>
#include <algorithm>
#include <memory>

namespace harmonies {
namespace core {

Game::Game(const model::GameConfig &gameConfig, const std::vector<std::string> &playerNames)
    : config(gameConfig),
      tokenBag(),
      centralBoard(gameConfig.getNbPlayer() == 1 ? 3 : 5),
      turnManager(nullptr),
      state(GameState::NotStarted)
{
    if (playerNames.size() != gameConfig.getNbPlayer())
    {
        throw std::invalid_argument("Game: le nombre de joueurs ne correspond pas a la configuration.");
    }
    for (const auto &name : playerNames)
    {
        players.push_back(std::make_unique<model::Player>(name, config.getSide()));
    }
}

void Game::initGame()
{
    if (state != GameState::NotStarted)
    {
        throw std::logic_error("Game::initGame: Impossible d'initialiser une partie deja demarree.");
    }

    std::vector<model::Player *> rawPlayers;
    rawPlayers.reserve(players.size());
    for (const auto &p : players)
    {
        rawPlayers.push_back(p.get());
    }
    turnManager = std::make_unique<core::TurnManager>(rawPlayers);

    state = GameState::WaitingForSlotChoice;
}

GameState Game::getState() const
{
    return state;
}

model::Player *Game::getCurrentPlayer() const
{
    if (state == GameState::NotStarted || !turnManager)
    {
        return nullptr;
    }
    return turnManager->getCurrentPlayer();
}

core::TurnManager *Game::getTurnManager() const
{
    return turnManager.get();
}

model::CentralBoard *Game::getCentralBoard()
{
    return &centralBoard;
}

const model::CentralBoard *Game::getCentralBoard() const
{
    return &centralBoard;
}

model::TokenBag *Game::getTokenBag()
{
    return &tokenBag;
}

const model::TokenBag *Game::getTokenBag() const
{
    return &tokenBag;
}

const std::vector<std::unique_ptr<model::Player>> &Game::getPlayers() const
{
    return players;
}

bool Game::isGameOver() const
{
    return state == GameState::GameOver;
}

bool Game::takeTokensFromSlot(std::size_t slotIndex)
{
    if (state != GameState::WaitingForSlotChoice && state != GameState::LastTurn)
    {
        throw std::logic_error("Game::takeTokensFromSlot: Action illegale dans la phase actuelle.");
    }

    // Utilisation correcte du service SlotSelectionService instancié
    SlotSelectionService slotService;
    context.pendingTokens = slotService.selectSlot(centralBoard, tokenBag, slotIndex);

    state = GameState::WaitingForPlacement;
    return true;
}

bool Game::placeTokenOnBoard(const utils::HexCoord &coord, model::TokenType token)
{
    if (state != GameState::WaitingForPlacement)
    {
        throw std::logic_error("Game::placeTokenOnBoard: Action illegale.");
    }

    auto it = std::find(context.pendingTokens.begin(), context.pendingTokens.end(), token);
    if (it == context.pendingTokens.end())
    {
        return false;
    }

    model::Player* currentPlayer = getCurrentPlayer();
    if (!currentPlayer)
    {
        return false;
    }

    model::PersonalBoard *board = currentPlayer->getBoard();
    if (!board)
    {
        return false;
    }

    // Utilisation correcte du service TokenPlacementService instancié
    TokenPlacementService placementService;
    bool done = placementService.placeToken(*board, coord, token);

    if (done)
    {
        context.pendingTokens.erase(it);
        if (context.pendingTokens.empty())
        {
            turnManager->nextTurn();
            state = GameState::WaitingForSlotChoice;
            checkEndGame();
        }
    }
    return done;
}

void Game::checkEndGame()
{
    std::vector<const model::PersonalBoard *> boards;
    boards.reserve(players.size());
    for (const auto &player : players)
    {
        boards.push_back(player->getBoard());
    }

    // Utilisation de la règle via EndGameChecker instancié ou statique selon vos rules
    rules::EndGameChecker checker;
    if (checker.isGameOver(boards, tokenBag))
    {
        if (state != GameState::LastTurn)
        {
            state = GameState::LastTurn;
        }
    }

    if (state == GameState::LastTurn && !players.empty() && getCurrentPlayer() == players.front().get())
    {
        state = GameState::GameOver;
    }
}

} // namespace core
} // namespace harmonies
