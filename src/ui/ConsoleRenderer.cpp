#include "ui/ConsoleRenderer.h"
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <map>

namespace harmonies
{
    namespace ui
    {

        char ConsoleRenderer::tokenToChar(model::TokenType type)
        {
            switch (type)
            {
            case model::TokenType::BlueWater:   return 'W';
            case model::TokenType::GrayStone:   return 'S';
            case model::TokenType::BrownEarth:  return 'E';
            case model::TokenType::GreenTree:   return 'T';
            case model::TokenType::YellowField: return 'F';
            case model::TokenType::RedBuilding: return 'B';
            default: return '?';
            }
        }


        void ConsoleRenderer::displayCentralBoard(const model::CentralBoard &board) const
        {
            std::cout << "=== Central Board ===\n";
            for (std::size_t i = 0; i < board.getNbSlots(); ++i)
            {
                const model::TokenSlot *slot = board.getSlot(i);
                std::cout << "Slot " << i << ": ";
                if (!slot || slot->isEmpty())
                {
                    std::cout << "(empty)";
                }
                else
                {
                    for (model::TokenType t : slot->getTokens())
                        std::cout << tokenToChar(t) << ' ';
                }
                std::cout << '\n';
            }
        }


        void ConsoleRenderer::displayPersonalBoard(const model::PersonalBoard &board) const
        {
            const auto &cells = board.getCells();
            if (cells.empty())
            {
                std::cout << "(empty board)\n";
                return;
            }


            int qMin = INT_MAX, qMax = INT_MIN;
            std::map<int, int> colRMin, colRMax;
            for (const auto &entry : cells)
            {
                int q = entry.first.getQ();
                int r = entry.first.getR();
                if (q < qMin) qMin = q;
                if (q > qMax) qMax = q;
                if (colRMin.count(q) == 0)
                    colRMin[q] = colRMax[q] = r;
                else
                {
                    colRMin[q] = std::min(colRMin[q], r);
                    colRMax[q] = std::max(colRMax[q], r);
                }
            }

            int numCols = qMax - qMin + 1;

            

            int rMinEven = INT_MAX, rMaxEven = INT_MIN;
            int rMinOdd  = INT_MAX, rMaxOdd  = INT_MIN;
            for (const auto &pair : colRMin)
            {
                int idx = pair.first - qMin;
                if (idx % 2 == 0)
                {
                    rMinEven = std::min(rMinEven, pair.second);
                    rMaxEven = std::max(rMaxEven, colRMax.at(pair.first));
                }
                else
                {
                    rMinOdd = std::min(rMinOdd, pair.second);
                    rMaxOdd = std::max(rMaxOdd, colRMax.at(pair.first));
                }
            }

            int maxLine = 0;
            if (rMaxEven != INT_MIN) maxLine = std::max(maxLine, (rMaxEven - rMinEven) * 2);
            if (rMaxOdd  != INT_MIN) maxLine = std::max(maxLine, (rMaxOdd  - rMinOdd)  * 2 + 1);


            
            std::map<int, std::map<int, const model::BoardCell *>> grid;
            for (const auto &entry : cells)
            {
                int q   = entry.first.getQ();
                int r   = entry.first.getR();
                int idx = q - qMin;
                int vline = (idx % 2 == 0)
                    ? (r - rMinEven) * 2
                    : (r - rMinOdd)  * 2 + 1;
                grid[vline][idx] = &entry.second;
            }


            char side = (board.getSide() == model::BoardSide::A) ? 'A' : 'B';
            std::cout << "=== Personal Board (Side " << side << ") ===\n";

            for (int vline = 0; vline <= maxLine; ++vline)
            {
                
                int firstCell = -1, lastCell = -1;
                if (grid.count(vline))
                {
                    for (const auto &slot : grid.at(vline))
                    {
                        if (firstCell == -1) firstCell = slot.first;
                        lastCell = slot.first;
                    }
                }

                for (int idx = 0; idx < numCols; ++idx)
                {
                    bool hasCell = grid.count(vline) && grid.at(vline).count(idx);
                    if (hasCell)
                    {
                        const model::BoardCell *cell = grid.at(vline).at(idx);
                        const auto &stack = cell->getTokenStack();
                        char c0 = stack.size() >= 1 ? tokenToChar(stack[0]) : '.';
                        char c1 = stack.size() >= 2 ? tokenToChar(stack[1]) : '.';
                        char c2 = stack.size() >= 3 ? tokenToChar(stack[2]) : '.';
                        if (cell->hasCube())
                        {
                            if      (stack.size() >= 3) c2 = static_cast<char>(std::tolower(static_cast<unsigned char>(c2)));
                            else if (stack.size() >= 2) c1 = static_cast<char>(std::tolower(static_cast<unsigned char>(c1)));
                            else if (stack.size() >= 1) c0 = static_cast<char>(std::tolower(static_cast<unsigned char>(c0)));
                        }
                        std::cout << "[" << c0 << c1 << c2 << "]";
                    }
                    else if (firstCell != -1 && idx > firstCell && idx < lastCell)
                    {
                        std::cout << "   |   ";
                    }
                    else
                    {
                        std::cout << "      ";
                    }
                }
                std::cout << '\n';
            }
            std::cout << "Legend: W=Water S=Stone E=Earth T=Tree F=Field B=Building ; lowercase=cube on top  [...]=unfilled slot\n";
            std::cout << "|: Separator between columns\n";
        }


        void ConsoleRenderer::displayScoreReport(const model::ScoreReport &report, const std::string &playerName) const
        {
            std::cout << "=== Score Report: " << playerName << " ===\n";
            std::cout << "  Trees:     " << report.getTreeScore()    << '\n';
            std::cout << "  Mountains: " << report.getMountainScore()<< '\n';
            std::cout << "  Fields:    " << report.getFieldScore()   << '\n';
            std::cout << "  Buildings: " << report.getBuildingScore()<< '\n';
            std::cout << "  Water:     " << report.getWaterScore()   << '\n';
            std::cout << "  Animals:   " << report.getAnimalsScore() << '\n';
            std::cout << "  Spirit:    " << report.getSpiritScore()  << '\n';
            std::cout << "  TOTAL:     " << report.getTotalScore()   << '\n';
        }


        void ConsoleRenderer::displayEndGame(const std::vector<model::Player *> &players) const
        {
            std::vector<model::Player *> ranked(players);
            std::sort(ranked.begin(), ranked.end(), [](model::Player *a, model::Player *b) {
                return a->getScore() > b->getScore();
            });


            std::cout << "=== Game Over -- Final Rankings ===\n";
            for (std::size_t i = 0; i < ranked.size(); ++i)
            {
                std::cout << (i + 1) << ". " << ranked[i]->getName()
                          << " -- " << ranked[i]->getScore() << " pts";
                if (i == 0)
                    std::cout << "  ***WINNER***";
                std::cout << '\n';
            }
        }

    } // namespace ui
} // namespace harmonies
