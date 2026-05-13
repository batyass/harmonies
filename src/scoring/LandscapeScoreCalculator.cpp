#include "scoring/LandscapeScoreCalculator.h"

#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <vector>

namespace harmonies
{
    namespace scoring
    {
        namespace
        {
            std::size_t stackHeightScore(std::size_t height)
            {
                if (height == 1)
                    return 1;
                if (height == 2)
                    return 3;
                if (height == 3)
                    return 7;
                return 0;
            }

            std::size_t riverScore(std::size_t length)
            {
                if (length <= 1)
                    return 0;
                if (length == 2)
                    return 2;
                if (length == 3)
                    return 5;
                if (length == 4)
                    return 8;
                if (length == 5)
                    return 11;
                if (length == 6)
                    return 15;
                return 15 + 4 * (length - 6);
            }

            bool topTokenIs(const model::BoardCell &cell, model::TokenType token)
            {
                return cell.getHeight() > 0 && cell.getTokenStack().back() == token;
            }

            std::size_t computeTreeScore(const model::PersonalBoard &board)
            {
                std::size_t score = 0;
                const std::map<utils::HexCoord, model::BoardCell> &cells = board.getCells();

                for (std::map<utils::HexCoord, model::BoardCell>::const_iterator it = cells.begin(); it != cells.end(); ++it)
                {
                    const model::BoardCell &cell = it->second;
                    if (rules::StackRule(cell) == model::StackType::Tree && topTokenIs(cell, model::TokenType::GreenTree))
                    {
                        score += stackHeightScore(cell.getHeight());
                    }
                }

                return score;
            }

            std::size_t computeMountainScore(const model::PersonalBoard &board)
            {
                std::size_t score = 0;
                const std::map<utils::HexCoord, model::BoardCell> &cells = board.getCells();

                for (std::map<utils::HexCoord, model::BoardCell>::const_iterator it = cells.begin(); it != cells.end(); ++it)
                {
                    const utils::HexCoord &coord = it->first;
                    const model::BoardCell &cell = it->second;

                    if (rules::StackRule(cell) != model::StackType::Mountain)
                    {
                        continue;
                    }

                    std::vector<const model::BoardCell *> neighbors = board.getAdjacentCells(coord);
                    bool hasMountainNeighbor = false;

                    for (std::size_t i = 0; i < neighbors.size(); ++i)
                    {
                        if (rules::StackRule(*neighbors[i]) == model::StackType::Mountain)
                        {
                            hasMountainNeighbor = true;
                            break;
                        }
                    }

                    if (hasMountainNeighbor)
                    {
                        score += stackHeightScore(cell.getHeight());
                    }
                }

                return score;
            }

            std::size_t computeFieldScore(const model::PersonalBoard &board)
            {
                std::size_t score = 0;
                std::map<utils::HexCoord, bool> visited;
                const std::map<utils::HexCoord, model::BoardCell> &cells = board.getCells();

                for (std::map<utils::HexCoord, model::BoardCell>::const_iterator it = cells.begin(); it != cells.end(); ++it)
                {
                    const utils::HexCoord &start = it->first;
                    const model::BoardCell &cell = it->second;

                    if (visited[start] || rules::StackRule(cell) != model::StackType::Field)
                    {
                        continue;
                    }

                    std::queue<utils::HexCoord> frontier;
                    frontier.push(start);
                    visited[start] = true;
                    std::size_t componentSize = 0;

                    while (!frontier.empty())
                    {
                        utils::HexCoord current = frontier.front();
                        frontier.pop();
                        ++componentSize;

                        std::vector<const model::BoardCell *> neighbors = board.getAdjacentCells(current);
                        for (std::size_t i = 0; i < neighbors.size(); ++i)
                        {
                            const utils::HexCoord &neighborCoord = neighbors[i]->getCoordinate();
                            if (!visited[neighborCoord] && rules::StackRule(*neighbors[i]) == model::StackType::Field)
                            {
                                visited[neighborCoord] = true;
                                frontier.push(neighborCoord);
                            }
                        }
                    }

                    if (componentSize >= 2)
                    {
                        score += 5;
                    }
                }

                return score;
            }

            std::size_t computeBuildingScore(const model::PersonalBoard &board)
            {
                std::size_t score = 0;
                const std::map<utils::HexCoord, model::BoardCell> &cells = board.getCells();

                for (std::map<utils::HexCoord, model::BoardCell>::const_iterator it = cells.begin(); it != cells.end(); ++it)
                {
                    const utils::HexCoord &coord = it->first;
                    const model::BoardCell &cell = it->second;

                    if (rules::StackRule(cell) != model::StackType::Building)
                    {
                        continue;
                    }

                    std::vector<const model::BoardCell *> neighbors = board.getAdjacentCells(coord);
                    std::set<model::TokenType> neighborColors;

                    for (std::size_t i = 0; i < neighbors.size(); ++i)
                    {
                        if (neighbors[i]->getHeight() > 0)
                        {
                            neighborColors.insert(neighbors[i]->getTokenStack().back());
                        }
                    }

                    if (neighborColors.size() >= 3)
                    {
                        score += 5;
                    }
                }

                return score;
            }

            std::size_t computeLongestRiverLength(const model::PersonalBoard &board)
            {
                std::size_t longest = 0;
                std::map<utils::HexCoord, bool> visited;
                const std::map<utils::HexCoord, model::BoardCell> &cells = board.getCells();

                for (std::map<utils::HexCoord, model::BoardCell>::const_iterator it = cells.begin(); it != cells.end(); ++it)
                {
                    const utils::HexCoord &start = it->first;
                    const model::BoardCell &cell = it->second;

                    if (visited[start] || rules::StackRule(cell) != model::StackType::Water)
                    {
                        continue;
                    }

                    std::vector<utils::HexCoord> component;
                    std::queue<utils::HexCoord> frontier;
                    frontier.push(start);
                    visited[start] = true;

                    while (!frontier.empty())
                    {
                        utils::HexCoord current = frontier.front();
                        frontier.pop();
                        component.push_back(current);

                        std::vector<const model::BoardCell *> neighbors = board.getAdjacentCells(current);
                        for (std::size_t i = 0; i < neighbors.size(); ++i)
                        {
                            const utils::HexCoord &neighborCoord = neighbors[i]->getCoordinate();
                            if (!visited[neighborCoord] && rules::StackRule(*neighbors[i]) == model::StackType::Water)
                            {
                                visited[neighborCoord] = true;
                                frontier.push(neighborCoord);
                            }
                        }
                    }

                    for (std::size_t i = 0; i < component.size(); ++i)
                    {
                        std::map<utils::HexCoord, std::size_t> distance;
                        std::queue<utils::HexCoord> bfs;
                        bfs.push(component[i]);
                        distance[component[i]] = 1;

                        while (!bfs.empty())
                        {
                            utils::HexCoord current = bfs.front();
                            bfs.pop();

                            std::vector<const model::BoardCell *> neighbors = board.getAdjacentCells(current);
                            for (std::size_t j = 0; j < neighbors.size(); ++j)
                            {
                                const utils::HexCoord &neighborCoord = neighbors[j]->getCoordinate();
                                if (rules::StackRule(*neighbors[j]) == model::StackType::Water && distance.find(neighborCoord) == distance.end())
                                {
                                    distance[neighborCoord] = distance[current] + 1;
                                    bfs.push(neighborCoord);
                                }
                            }
                        }

                        for (std::map<utils::HexCoord, std::size_t>::const_iterator distIt = distance.begin(); distIt != distance.end(); ++distIt)
                        {
                            longest = std::max(longest, distIt->second);
                        }
                    }
                }

                return longest;
            }

            std::size_t computeIslandScore(const model::PersonalBoard &board)
            {
                std::size_t islandCount = 0;
                std::map<utils::HexCoord, bool> visited;
                const std::map<utils::HexCoord, model::BoardCell> &cells = board.getCells();

                for (std::map<utils::HexCoord, model::BoardCell>::const_iterator it = cells.begin(); it != cells.end(); ++it)
                {
                    const utils::HexCoord &start = it->first;
                    const model::BoardCell &cell = it->second;

                    if (visited[start] || rules::StackRule(cell) == model::StackType::Water)
                    {
                        continue;
                    }

                    std::queue<utils::HexCoord> frontier;
                    frontier.push(start);
                    visited[start] = true;
                    ++islandCount;

                    while (!frontier.empty())
                    {
                        utils::HexCoord current = frontier.front();
                        frontier.pop();

                        std::vector<const model::BoardCell *> neighbors = board.getAdjacentCells(current);
                        for (std::size_t i = 0; i < neighbors.size(); ++i)
                        {
                            const utils::HexCoord &neighborCoord = neighbors[i]->getCoordinate();
                            if (!visited[neighborCoord] && rules::StackRule(*neighbors[i]) != model::StackType::Water)
                            {
                                visited[neighborCoord] = true;
                                frontier.push(neighborCoord);
                            }
                        }
                    }
                }

                return islandCount * 5;
            }
        } // namespace anonyme

        std::size_t LandscapeScoreCalculator(const model::PersonalBoard &board)
        {
            std::size_t score = 0;

            score += computeTreeScore(board);
            score += computeMountainScore(board);
            score += computeFieldScore(board);
            score += computeBuildingScore(board);

            if (board.getSide() == model::BoardSide::A)
            {
                score += riverScore(computeLongestRiverLength(board));
            }
            else
            {
                score += computeIslandScore(board);
            }

            return score;
        }

    } // namespace scoring
} // namespace harmonies
