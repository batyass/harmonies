#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <stdexcept>
#include "model/Player.h"
#include "model/GameConfig.h"
#include "model/TokenBag.h"
#include "model/CentralBoard.h"
#include "core/TurnManager.h"
#include "core/GameState.h"

namespace harmonies {
namespace core {

class Game {
private:
    model::GameConfig config;
    std::vector<model::Player*> players;
    
    // Agrégation par valeur (RAII) au lieu de pointeurs nus
    model::TokenBag tokenBag;
    model::CentralBoard centralBoard;
    
    core::TurnManager* turnManager;

    GamePhase phase;

public:
    Game(const model::GameConfig& gameConfig, const std::vector<std::string>& playerNames);
    ~Game();

    // ------------------------------------------------------------------------
    // Règle des 5 : Interdiction stricte de copier ou d'assigner l'objet Game.
    // Cela prévient les failles de type "double free" sur les pointeurs des joueurs.
    // ------------------------------------------------------------------------
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    void initGame();

    // Accesseurs pour l'UI
    GamePhase getPhase() const;
    model::Player* getCurrentPlayer() const;
    core::TurnManager* getTurnManager() const;
    
    // Les méthodes retournent l'adresse de l'objet agrégé
    model::CentralBoard* getCentralBoard();
    model::TokenBag* getTokenBag();
    
    const std::vector<model::Player*>& getPlayers() const;

    bool isGameOver() const;

    // Actions de base (squelettes)
    bool takeTokensFromMarket(std::size_t slotIndex);
};

}
}

#endif