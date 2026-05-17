#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <stdexcept>
#include <memory>
#include "model/Player.h"
#include "model/GameConfig.h"
#include "model/TokenBag.h"
#include "model/CentralBoard.h"
#include "core/TurnManager.h"
#include "core/GameState.h"

namespace harmonies
{
    namespace core
    {

        class Game
        {
        private:
            model::GameConfig config;
            std::vector<std::unique_ptr<model::Player>> players;

            // Agrégation par valeur (RAII) au lieu de pointeurs nus
            model::TokenBag tokenBag;
            model::CentralBoard centralBoard;

            std::unique_ptr<core::TurnManager> turnManager;

            GameState state;

        public:
            Game(const model::GameConfig &gameConfig, const std::vector<std::string> &playerNames);
            ~Game() = default;

            // ------------------------------------------------------------------------
            // Règle des 5 : Interdiction stricte de copier ou d'assigner l'objet Game.
            // Cela prévient les failles de type "double free" sur les pointeurs des joueurs.
            // ------------------------------------------------------------------------
            Game(const Game &) = delete;
            Game &operator=(const Game &) = delete;
            Game(Game &&) = delete;
            Game &operator=(Game &&) = delete;

            void initGame();

            // Accesseurs pour l'UI
            GameState getState() const;
            model::Player *getCurrentPlayer() const;
            core::TurnManager *getTurnManager() const;

            // Les méthodes retournent l'adresse de l'objet agrégé
            model::CentralBoard *getCentralBoard();
            const model::CentralBoard *getCentralBoard() const;
            model::TokenBag *getTokenBag();
            const model::TokenBag *getTokenBag() const;

            const std::vector<std::unique_ptr<model::Player>> &getPlayers() const;

            bool isGameOver() const;

            // Actions de base (squelettes)
            bool takeTokensFromSlot(std::size_t slotIndex);

            bool placeTokenOnBoard(const utils::HexCoord &coord, model::TokenType token);
            void checkEndGame();
        };

    }
}

#endif