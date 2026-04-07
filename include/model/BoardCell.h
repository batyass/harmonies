#ifndef BOARDCELL_H
#define BOARDCELL_H

#include "utils/HexCoord.h"
#include "model/TokenType.h"
#include <vector>

namespace harmonies {
namespace model {

class BoardCell {
private:
    utils::HexCoord coordinate;
    std::vector<TokenType> tokenStack; // Max 3 tokens
    bool hasAnimalCube;                // "au plus 1 cube" per rules
    
public:
    BoardCell(utils::HexCoord coord);
    
    // Getters
    utils::HexCoord getCoordinate() const { return coordinate; }
    bool isOccupiedByCube() const { return hasAnimalCube; }
    int getHeight() const { return tokenStack.size(); }
    
    // Logic for stacking
    bool addToken(TokenType type);
    void placeAnimalCube();
};

} // namespace model
} // namespace harmonies

#endif