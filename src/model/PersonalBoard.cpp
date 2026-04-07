#include "model/PersonalBoard.h"
#include <cmath>

namespace harmonies {
namespace model {

PersonalBoard::PersonalBoard(BoardSide s) : side(s) {
    initializeGrid();
}

PersonalBoard::~PersonalBoard() {
    // Clean up allocated memory for BoardCell objects
    for (auto& pair : cells) {
        delete pair.second;
    }
    cells.clear();
}

void PersonalBoard::initializeGrid() {
    // Standard radius for Harmonies board is typically 3 
    // (from center to edge, total 37 cells)
    int radius = 3;

    // Clear existing cells if any (for safety)
    for (auto& pair : cells) delete pair.second;
    cells.clear();

    // Logic for A and B (currently they share the same hexagonal shape 
    // but might differ in "special" cell properties later)
    if (side == BoardSide::A || side == BoardSide::B) {
        for (int q = -radius; q <= radius; ++q) {
            int r1 = std::max(-radius, -q - radius);
            int r2 = std::min(radius, -q + radius);
            for (int r = r1; r <= r2; ++r) {
                utils::HexCoord coord(q, r);
                cells[coord] = new BoardCell(coord);
            }
        }
    }
    
    // Future expansion:
    // else if (side == BoardSide::C) { ... custom shape logic ... }
}

BoardCell* PersonalBoard::getCell(utils::HexCoord coord) {
    auto it = cells.find(coord);
    if (it != cells.end()) {
        return it->second;
    }
    return nullptr; // Return nullptr if coord is outside the board
}

bool PersonalBoard::placeToken(utils::HexCoord coord, TokenType type) {
    BoardCell* cell = getCell(coord);
    if (cell != nullptr) {
        return cell->addToken(type);
    }
    return false; // Cell doesn't exist at this coordinate
}

} // namespace model
} // namespace harmonies