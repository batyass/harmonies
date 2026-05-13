#ifndef PERSONALBOARD_H
#define PERSONALBOARD_H

#include <map>
#include <vector>

#include "model/BoardSide.h"
#include "model/BoardCell.h"
#include "utils/hexCoord.h"

namespace harmonies
{
    namespace model
    {

        class PersonalBoard
        {
        private:
            BoardSide side;
            // We use a map to store cells, indexed by their HexCoord
            std::map<utils::HexCoord, BoardCell> cells;

            // Initialize the board shape based on side A or B
            void initializeGrid();

        public:
            explicit PersonalBoard(BoardSide s);

            BoardSide getSide() const { return side; }

            // Add a token to a specific cell
            bool contains(const utils::HexCoord &coord) const;
            bool placeToken(const utils::HexCoord &coord, TokenType type);

            // Check if a cell exists and get it
            BoardCell *getCell(const utils::HexCoord &coord);
            const BoardCell *getCell(const utils::HexCoord &coord) const;
            const std::map<utils::HexCoord, BoardCell> &getCells() const;
            std::vector<model::BoardCell *> getAdjacentCells(const utils::HexCoord &coord);
            std::vector<const model::BoardCell *> getAdjacentCells(const utils::HexCoord &coord) const;
        };

    } // namespace model
} // namespace harmonies

#endif
