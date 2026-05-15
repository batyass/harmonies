#include "core/Game.h"
#include <stdexcept>

namespace harmonies {
namespace core {

Game::Game(const model::GameConfig& gameConfig, const std::vector<std::string>& playerNames)
    : config(gameConfig), 
      tokenBag(), 
      centralBoard(gameConfig.getNbPlayer() == 1 ? 3 : 5), 
      turnManager(nullptr), 
      phase(GamePhase::NotStarted) {
    
    for (const auto& name : playerNames) {
        players.push_back(new model::Player(name, config.getSide()));
    }
}

Game::~Game() {
    for (model::Player* p : players) {
        delete p;
    }
    players.clear();

    delete turnManager;
    // tokenBag et centralBoard sont nettoyés automatiquement par le compilateur (RAII)
}

void Game::initGame() {
    if (phase != GamePhase::NotStarted) {
        throw std::logic_error("Game::initGame: Impossible d'initialiser une partie deja demarree.");
    }

    turnManager = new core::TurnManager(players);

    // Verrouillage de l'état
    phase = GamePhase::WaitingForMarketChoice;
}

GamePhase Game::getPhase() const {
    return phase;
}

model::Player* Game::getCurrentPlayer() const {
    if (phase == GamePhase::NotStarted) return nullptr;
    return turnManager->getCurrentPlayer();
}

core::TurnManager* Game::getTurnManager() const {
    return turnManager;
}

model::CentralBoard* Game::getCentralBoard() {
    return &centralBoard;
}

model::TokenBag* Game::getTokenBag() {
    return &tokenBag;
}

const std::vector<model::Player*>& Game::getPlayers() const {
    return players;
}

bool Game::isGameOver() const {
    return phase == GamePhase::GameOver;
}

bool Game::takeTokensFromMarket(std::size_t slotIndex) {
    // FSM Hermétique : l'action est formellement interdite si on n'est pas dans la bonne phase
    if (phase != GamePhase::WaitingForMarketChoice) {
        throw std::logic_error("Game::takeTokensFromMarket: Action illegale dans la phase actuelle.");
    }
    
    // Logique de récupération des jetons à implémenter par le développeur
    // ...

    // Transition obligatoire une fois l'action réalisée
    // phase = GamePhase::WaitingForPlacement;
    
    return true;
}

}
}