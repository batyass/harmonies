#include "core/TurnManager.h"
#include <utility>

namespace harmonies {
namespace core {

TurnManager::TurnManager(const std::vector<model::Player*>& gamePlayers)
    : players(gamePlayers), currentPlayerIndex(0), currentTurn(1) {
    if (players.empty()) {
        throw std::invalid_argument("TurnManager: player list cannot be empty");
    }
}

TurnManager::~TurnManager() {
}

model::Player* TurnManager::getCurrentPlayer() const {
    return players[currentPlayerIndex];
}

void TurnManager::nextTurn() {
    currentPlayerIndex++;

    if (currentPlayerIndex >= players.size()) {
        currentPlayerIndex = 0;
        currentTurn++;
    }
}

unsigned int TurnManager::getTurnCount() const {
    return currentTurn;
}

bool TurnManager::isNewCycle() const {
    return currentPlayerIndex == 0;
}

}
}
