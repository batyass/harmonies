#include "model/PersonalBoard.h"

#include <array>
#include <utility>
#include <vector>

namespace harmonies
{
    namespace model
    {

        PersonalBoard::PersonalBoard(BoardSide s) : side(s)
        {
            initializeGrid();
        }

        namespace // namespace anonyme
        {
            struct ColumnSpec
            {
                int q;      // colonne
                int rStart; // première ligne de la colonne
                int count;  // nombre de cases dans cette colonne
            };
        }

        void PersonalBoard::initializeGrid()
        {
            cells.clear();

            std::vector<ColumnSpec> columns;

            if (side == BoardSide::A)
            {
                // Face A : 5, 4, 5, 4, 5 = 23 cases
                columns = {
                    {-2, -2, 5},
                    {-1, -1, 4},
                    {0, -2, 5},
                    {1, -1, 4},
                    {2, -2, 5}};
            }
            else if (side == BoardSide::B)
            {
                // Face B : 4, 3, 4, 3, 4, 3, 4 = 25 cases
                columns = {
                    {-3, -2, 4},
                    {-2, -1, 3},
                    {-1, -2, 4},
                    {0, -1, 3},
                    {1, -2, 4},
                    {2, -1, 3},
                    {3, -2, 4}};
            }
            else
            {
                return;
            }

            for (const ColumnSpec &column : columns)
            {
                for (int i = 0; i < column.count; ++i)
                {
                    int q = column.q;
                    int r = column.rStart + i;

                    utils::HexCoord coord(q, r);
                    cells.emplace(coord, BoardCell(coord));
                }
            }
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

        const std::map<utils::HexCoord, BoardCell> &PersonalBoard::getCells() const
        {
            return cells;
        }

        std::vector<model::BoardCell *> PersonalBoard::getAdjacentCells(const utils::HexCoord &coord)
        {
            static const std::array<std::pair<int, int>, 6> directions = {{
                std::make_pair(1, 0),
                std::make_pair(1, -1),
                std::make_pair(0, -1),
                std::make_pair(-1, 0),
                std::make_pair(-1, 1),
                std::make_pair(0, 1)}};

            std::vector<model::BoardCell *> adjacentCells;

            for (std::size_t i = 0; i < directions.size(); ++i)
            {
                int dq = directions[i].first;
                int dr = directions[i].second;
                utils::HexCoord neighbor(coord.getQ() + dq, coord.getR() + dr);

                auto it = cells.find(neighbor);

                if (it != cells.end())
                {
                    adjacentCells.push_back(&it->second);
                }
            }

            return adjacentCells;
        }

        std::vector<const model::BoardCell *> PersonalBoard::getAdjacentCells(const utils::HexCoord &coord) const
        {
            static const std::array<std::pair<int, int>, 6> directions = {{
                std::make_pair(1, 0),
                std::make_pair(1, -1),
                std::make_pair(0, -1),
                std::make_pair(-1, 0),
                std::make_pair(-1, 1),
                std::make_pair(0, 1)}};

            std::vector<const model::BoardCell *> adjacentCells;

            for (std::size_t i = 0; i < directions.size(); ++i)
            {
                int dq = directions[i].first;
                int dr = directions[i].second;
                utils::HexCoord neighbor(coord.getQ() + dq, coord.getR() + dr);

                auto it = cells.find(neighbor);

                if (it != cells.end())
                {
                    adjacentCells.push_back(&it->second);
                }
            }

            return adjacentCells;
        }

    } // namespace model
} // namespace harmonies
