#include "model/BoardCell.h"

namespace harmonies {
namespace model {

BoardCell::BoardCell(utils::HexCoord coord) 
    : coordinate(coord), hasAnimalCube(false) {}

bool BoardCell::addToken(TokenType type) {
    // Teammate's rule: Max 3 tokens
    if (tokenStack.size() >= 3) return false;
    
    // Teammate's rule: Cannot stack if there is already an animal cube
    if (hasAnimalCube) return false;

    // TODO: Add StackRule logic here later (e.g., Water cannot be stacked)
    tokenStack.push_back(type);
    return true;
}

void BoardCell::placeAnimalCube() {
    if (!hasAnimalCube) {
        hasAnimalCube = true;
    }
}

} // namespace model
} // namespace harmonies