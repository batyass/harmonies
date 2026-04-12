#include "model/PersonalBoard.h"
#include <cmath>

namespace harmonies
{
    namespace model
    {

        PersonalBoard::PersonalBoard(BoardSide s) : side(s)
        {
            initializeGrid();
        }

        void PersonalBoard::initializeGrid()
        {
            // Standard radius for Harmonies board is typically 3
            // (from center to edge, total 37 cells)
            int radius = 3;

            cells.clear();

            // Logic for A and B (currently they share the same hexagonal shape
            // but might differ in "special" cell properties later)
            if (side == BoardSide::A || side == BoardSide::B)
            {
                for (int q = -radius; q <= radius; ++q)
                {
                    int r1 = std::max(-radius, -q - radius);
                    int r2 = std::min(radius, -q + radius);
                    for (int r = r1; r <= r2; ++r)
                    {
                        utils::HexCoord coord(q, r);
                        cells.emplace(coord, BoardCell(coord));
                    }
                }
            }

            // Future expansion:
            // else if (side == BoardSide::C) { ... custom shape logic ... }
        }

        BoardCell *PersonalBoard::getCell(const utils::HexCoord &coord)
        {
            auto it = cells.find(coord);
            if (it != cells.end())
            {
                return &it->second;
            }
            return nullptr;
        }

        const BoardCell *PersonalBoard::getCell(const utils::HexCoord &coord) const
        {
            auto it = cells.find(coord);
            if (it != cells.end())
            {
                return &it->second;
            }
            return nullptr;
        }

        bool PersonalBoard::contains(const utils::HexCoord &coord) const
        {
            return cells.find(coord) != cells.end();
        }

        bool PersonalBoard::placeToken(const utils::HexCoord &coord, TokenType type)
        {
            BoardCell *cell = getCell(coord);
            if (cell != nullptr)
            {
                return cell->addToken(type);
            }
            return false; // Cell doesn't exist at this coordinate
        }

    } // namespace model
} // namespace harmonies
