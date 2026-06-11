#include "core/Game.h"
#include <algorithm>
#include <stdexcept>

#include "core/NatureSpiritPlacementService.h"
#include "core/DefaultDeckFactory.h"
#include "rules/EndGameChecker.h"
#include "core/AnimalCubePlacementService.h"
#include "core/SlotSelectionService.h"
#include "core/TokenPlacementService.h"
#include "scoring/AnimalCardScoreCalculator.h"
#include "scoring/LandscapeScoreCalculator.h"
#include "scoring/NatureSpiritScoreCalculator.h"

namespace harmonies
{
    namespace core
    {
        namespace
        {
            void finalizeScores(std::vector<std::unique_ptr<model::Player>> &players,
                                const Game &game)
            {
                for (std::size_t i = 0; i < players.size(); ++i)
                {
                    players[i]->setScore(static_cast<int>(game.calculatePlayerScore(*players[i])));
                }
            }
        }

        Game::Game(const model::GameConfig &gameConfig, const std::vector<std::string> &playerNames)
            : config(gameConfig),
              tokenBag(),
              centralBoard(gameConfig.getNbPlayer() == 1 ? 3 : 5),
              animalCardDeck(makeDefaultAnimalCards(), visibleAnimalCardSlots(gameConfig.getNbPlayer())),
              natureSpiritDeck(makeDefaultNatureSpiritCards()),
              turnManager(),
              state(GameState::NotStarted),
              finalRoundTriggered(false)
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

            if (config.getNatureSpiritOption())
            {
                std::vector<std::vector<model::NatureSpiritCard>> hands = natureSpiritDeck.deal(players.size());
                for (std::size_t i = 0; i < players.size(); ++i)
                {
                    for (std::size_t j = 0; j < hands[i].size(); ++j)
                    {
                        players[i]->addNatureSpiritCard(hands[i][j]);
                    }
                }
            }

            for (std::size_t i = 0; i < centralBoard.getNbSlots(); ++i)
            {
                model::TokenSlot *slot = centralBoard.getSlot(i);
                if (slot != nullptr)
                {
                    slot->fill(tokenBag.drawTokens(3));
                }
            }

            // Verrouillage de l'état
            state = GameState::WaitingForSlotChoice;
            finalRoundTriggered = false;
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

        std::size_t Game::calculatePlayerScore(const model::Player &player) const
        {
            // Temporaire : en attendant un ScoreCalculator dédié, Game agrège lui-même
            // les différentes catégories de score à partir des calculateurs existants.
            std::size_t total = scoring::LandscapeScoreCalculator(*player.getBoard());

            const std::vector<model::AnimalCard> &animalCards = player.getAnimalCards()->getCards();
            for (std::size_t i = 0; i < animalCards.size(); ++i)
            {
                total += scoring::AnimalCardScoreCalculator(animalCards[i]);
            }

            const std::vector<model::NatureSpiritCard> &natureSpiritCards = player.getNatureSpiritCards();
            for (std::size_t i = 0; i < natureSpiritCards.size(); ++i)
            {
                total += scoring::NatureSpiritScoreCalculator(natureSpiritCards[i], *player.getBoard());
            }

            return total;
        }

        const model::Player *Game::getWinner() const
        {
            if (players.empty())
            {
                return nullptr;
            }

            const model::Player *winner = players[0].get();
            for (std::size_t i = 1; i < players.size(); ++i)
            {
                if (players[i]->getScore() > winner->getScore())
                {
                    winner = players[i].get();
                }
            }

            return winner;
        }

        const std::vector<model::TokenType> &Game::getPendingTokens() const
        {
            return context.pendingTokens;
        }

        bool Game::isNatureSpiritEnabled() const
        {
            return config.getNatureSpiritOption();
        }

        bool Game::isFinalRoundTriggered() const
        {
            return finalRoundTriggered;
        }

        model::CentralBoard *Game::getCentralBoard()
        {
            return &centralBoard;
        }

        const model::CentralBoard *Game::getCentralBoard() const
        {
            return &centralBoard;
        }

        model::AnimalCardDeck *Game::getAnimalCardDeck()
        {
            return &animalCardDeck;
        }

        const model::AnimalCardDeck *Game::getAnimalCardDeck() const
        {
            return &animalCardDeck;
        }

        model::NatureSpiritDeck *Game::getNatureSpiritDeck()
        {
            return &natureSpiritDeck;
        }

        const model::NatureSpiritDeck *Game::getNatureSpiritDeck() const
        {
            return &natureSpiritDeck;
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

        void Game::finishTurn()
        {
            turnManager->nextTurn();
            animalCardDeck.refill();
            context.pendingTokens.clear();
            context.hasTakenAnimalCard = false;
            state = GameState::WaitingForSlotChoice;
            checkEndGame();
        }

        bool Game::takeTokensFromSlot(std::size_t slotIndex)
        {
            // FSM Hermétique : l'action est formellement interdite si on n'est pas dans la bonne phase
            if (state != GameState::WaitingForSlotChoice)
            {
                throw std::logic_error("Game::takeTokensFromMarket: Action illegale dans la phase actuelle.");
            }

            // Logique de récupération des jetons
            context.pendingTokens = core::takeTokensFromSlot(centralBoard, tokenBag, slotIndex);

            // Transition obligatoire une fois l'action réalisée
            state = GameState::WaitingForPlacement;

            return true;
        }

        bool Game::takeVisibleAnimalCard(std::size_t index)
        {
            if (state != GameState::WaitingForSlotChoice &&
                state != GameState::WaitingForPlacement &&
                state != GameState::WaitingForTurnEndChoice)
            {
                throw std::logic_error("Game::takeVisibleAnimalCard: Action illegale dans la phase actuelle.");
            }

            if (context.hasTakenAnimalCard)
            {
                return false;
            }

            static constexpr std::size_t MAX_ANIMAL_CARDS = 4;
            model::Player *cp = turnManager->getCurrentPlayer();
            if (cp->getAnimalCards()->getCardCount() + cp->getNatureSpiritCardCount() >= MAX_ANIMAL_CARDS)
            {
                return false;
            }

            model::AnimalCard takenCard = animalCardDeck.takeVisible(index);
            turnManager->getCurrentPlayer()->getAnimalCards()->addCard(takenCard);
            context.hasTakenAnimalCard = true;
            return true;
        }

        bool Game::placeAnimalCube(std::size_t cardIndex, const utils::HexCoord &anchor)
        {
            if (state != GameState::WaitingForSlotChoice &&
                state != GameState::WaitingForPlacement &&
                state != GameState::WaitingForTurnEndChoice)
            {
                throw std::logic_error("Game::placeAnimalCube: Action illegale dans la phase actuelle.");
            }

            model::Player *currentPlayer = turnManager->getCurrentPlayer();
            model::AnimalCard *card = currentPlayer->getAnimalCards()->getCard(cardIndex);
            if (card == nullptr)
            {
                return false;
            }

            return core::placeAnimalCube(*card, *currentPlayer->getBoard(), anchor);
        }

        bool Game::chooseNatureSpiritCard(std::size_t index)
        {
            if (!config.getNatureSpiritOption())
            {
                return false;
            }

            if (state != GameState::WaitingForSlotChoice &&
                state != GameState::WaitingForPlacement &&
                state != GameState::WaitingForTurnEndChoice)
            {
                throw std::logic_error("Game::chooseNatureSpiritCard: Action illegale dans la phase actuelle.");
            }

            model::Player *currentPlayer = turnManager->getCurrentPlayer();
            if (turnManager->getTurnCount() != 1)
            {
                return false;
            }
            if (currentPlayer->getNatureSpiritCardCount() <= 1)
            {
                return false;
            }

            return currentPlayer->chooseNatureSpiritCard(index);
        }

        bool Game::placeNatureSpiritCube(const utils::HexCoord &anchor)
        {
            if (!config.getNatureSpiritOption())
            {
                return false;
            }

            if (state != GameState::WaitingForSlotChoice &&
                state != GameState::WaitingForPlacement &&
                state != GameState::WaitingForTurnEndChoice)
            {
                throw std::logic_error("Game::placeNatureSpiritCube: Action illegale dans la phase actuelle.");
            }

            model::Player *currentPlayer = turnManager->getCurrentPlayer();
            if (currentPlayer->getNatureSpiritCardCount() != 1)
            {
                return false;
            }

            model::NatureSpiritCard *card = currentPlayer->getNatureSpiritCard(0);
            if (card == nullptr)
            {
                return false;
            }

            return core::placeNatureSpiritCube(*card, *currentPlayer->getBoard(), anchor);
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
                    state = GameState::WaitingForTurnEndChoice;
                }
                return done;
            }
            return false;
        }

        bool Game::endTurn()
        {
            if (state != GameState::WaitingForTurnEndChoice)
            {
                throw std::logic_error("Game::endTurn: Action illegale dans la phase actuelle.");
            }

            if (config.getNatureSpiritOption() &&
                turnManager->getTurnCount() == 1 &&
                turnManager->getCurrentPlayer()->getNatureSpiritCardCount() > 1)
            {
                return false;
            }

            finishTurn();
            return true;
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
                finalRoundTriggered = true;
            }
            if (finalRoundTriggered && (getCurrentPlayer() == players[0].get()))
            {
                state = GameState::GameOver;
                finalizeScores(players, *this);
            }
        }

    }
}
