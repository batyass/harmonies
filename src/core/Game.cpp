#include "core/Game.h"
#include <stdexcept>
#include <algorithm>
#include "rules/EndGameChecker.h"
#include "core/SlotSelectionService.h"
#include "core/TokenPlacementService.h"

namespace harmonies
{
    namespace core
    {

        Game::Game(const model::GameConfig &gameConfig, const std::vector<std::string> &playerNames)
            : config(gameConfig),
              tokenBag(),
              centralBoard(gameConfig.getNbPlayer() == 1 ? 3 : 5),
              turnManager(),
              state(GameState::NotStarted)
        {
            if (playerNames.size() != gameConfig.getNbPlayer())
            {
                throw std::invalid_argument("Game: wrong number of players");
            }
            for (const auto &name : playerNames)
            {
                players.push_back(std::unique_ptr<model::Player>(new model::Player(name, config.getSide())));
            }
        }

        void Game::initGame()
        {
            if (state != GameState::NotStarted)
            {
                throw std::logic_error("Game::initGame: Impossible d'initialiser une partie deja demarree.");
            }

            // TurnManager reçoit une vue non-propriétaire (raw pointers) ; Game reste seul propriétaire.
            std::vector<model::Player *> rawPlayers;
            rawPlayers.reserve(players.size());
            for (const auto &p : players)
            {
                rawPlayers.push_back(p.get());
            }
            turnManager = std::unique_ptr<core::TurnManager>(new core::TurnManager(rawPlayers));

            // Verrouillage de l'état
            state = GameState::WaitingForSlotChoice;
        }

        GameState Game::getState() const
        {
            return state;
        }

        model::Player *Game::getCurrentPlayer() const
        {
            if (state == GameState::NotStarted)
                return nullptr;
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
            // FSM Hermétique : l'action est formellement interdite si on n'est pas dans la bonne phase
            if (state != GameState::WaitingForSlotChoice && state != GameState::LastTurn)
            {
                throw std::logic_error("Game::takeTokensFromMarket: Action illegale dans la phase actuelle.");
            }

            // Logique de récupération des jetons
            context.pendingTokens = core::takeTokensFromSlot(centralBoard, tokenBag, slotIndex);

            // Transition obligatoire une fois l'action réalisée
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
            if (it != context.pendingTokens.end())
            {
                bool done = core::placeTokenOnBoard(*turnManager->getCurrentPlayer()->getBoard(), coord, token);
                if (done)
                    context.pendingTokens.erase(it);
                if (context.pendingTokens.empty())
                {
                    turnManager->nextTurn();
                    state = GameState::WaitingForSlotChoice;
                    checkEndGame();
                }
                return done;
            }
            return false;
        }

        void Game::checkEndGame()
        {
            std::vector<const model::PersonalBoard *> boards;
            for (const auto &player : players)
            {
                boards.push_back(player->getBoard());
            }
            if (rules::isGameOver(boards, tokenBag))
            {
                state = GameState::LastTurn;
            }
            if ((state == GameState::LastTurn) && (getCurrentPlayer() == players[0].get()))
            {
                state = GameState::GameOver;
            }
        }

    }
}