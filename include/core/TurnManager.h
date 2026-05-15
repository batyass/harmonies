#ifndef TURNMANAGER_H
#define TURNMANAGER_H

#include <vector>
#include <stdexcept>
#include "model/Player.h"

namespace harmonies {
namespace core {

class TurnManager {
private:
    std::vector<model::Player*> players;
    std::size_t currentPlayerIndex;
    unsigned int currentTurn;

public:
    TurnManager(const std::vector<model::Player*>& gamePlayers);
    ~TurnManager();

    model::Player* getCurrentPlayer() const;
    void nextTurn();
    unsigned int getTurnCount() const;
    bool isNewCycle() const;
};

}
}

#endif
