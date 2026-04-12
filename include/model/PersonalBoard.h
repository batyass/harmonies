#ifndef PERSONALBOARD_H
#define PERSONALBOARD_H

#include <map>
#include "model/BoardCell.h"
#include "utils/HexCoord.h"

namespace harmonies
{
    namespace model
    {

        enum class BoardSide
        {
            A,
            B
        };

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
        };

    } // namespace model
} // namespace harmonies

#endif
