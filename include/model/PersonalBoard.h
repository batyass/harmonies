#ifndef PERSONALBOARD_H
#define PERSONALBOARD_H

#include <map>
#include <vector>
#include "model/BoardCell.h"
#include "utils/HexCoord.h"

namespace harmonies {
namespace model {

enum class BoardSide { A, B };

class PersonalBoard {
private:
    BoardSide side;
    // We use a map to store cells, indexed by their HexCoord
    std::map<utils::HexCoord, BoardCell*> cells;

public:
    PersonalBoard(BoardSide s);
    ~PersonalBoard();

    BoardSide getSide() const { return side; }
    
    // Add a token to a specific cell
    bool placeToken(utils::HexCoord coord, TokenType type);
    
    // Check if a cell exists and get it
    BoardCell* getCell(utils::HexCoord coord);
    
    // Initialize the board shape based on side A or B
    void initializeGrid();
};

} // namespace model
} // namespace harmonies

#endif