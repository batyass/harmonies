#include "scoring/NatureSpiritScoreCalculator.h"

#include <map>
#include <queue>
#include <set>

namespace harmonies
{
    namespace scoring
    {
        namespace
        {
            bool matchesLandscapeRule(const model::BoardCell &cell, const scoring::LandscapeCountRule &rule)
            {
                if (cell.getHeight() == 0)
                {
                    return false;
                }

                if (cell.getTokenStack().back() != rule.type)
                {
                    return false;
                }

                return rule.requiredHeight == 0 || cell.getHeight() == rule.requiredHeight;
            }

            bool matchesConnectedType(const model::BoardCell &cell, model::TokenType type)
            {
                return cell.getHeight() > 0 && cell.getTokenStack().back() == type;
            }

            bool groupSizeMatchesRule(std::size_t groupSize, const scoring::ConnectedGroupRule &rule)
            {
                if (groupSize < rule.minGroupSize)
                {
                    return false;
                }

                return rule.maxGroupSize == 0 || groupSize <= rule.maxGroupSize;
            }

            std::size_t computeLandscapeCount(const scoring::NatureSpiritEffect &effect, const model::PersonalBoard &board)
            {
                std::size_t score = 0;
                const std::map<utils::HexCoord, model::BoardCell> &cells = board.getCells();

                for (std::map<utils::HexCoord, model::BoardCell>::const_iterator cellIt = cells.begin(); cellIt != cells.end(); ++cellIt)
                {
                    const model::BoardCell &cell = cellIt->second;
                    for (std::size_t i = 0; i < effect.landscapeRules.size(); ++i)
                    {
                        if (matchesLandscapeRule(cell, effect.landscapeRules[i]))
                        {
                            score += effect.landscapeRules[i].score;
                        }
                    }
                }

                return score;
            }

            std::size_t computeConnectedGroups(const scoring::NatureSpiritEffect &effect, const model::PersonalBoard &board)
            {
                std::size_t score = 0;
                std::vector<model::TokenType> processedTypes;
                const std::vector<scoring::ConnectedGroupRule> &rules = effect.connectedRules;

                for (std::size_t i = 0; i < rules.size(); ++i)
                {
                    model::TokenType currentType = rules[i].type;
                    bool alreadyProcessed = false;
                    for (std::size_t j = 0; j < processedTypes.size(); ++j)
                    {
                        if (processedTypes[j] == currentType)
                        {
                            alreadyProcessed = true;
                            break;
                        }
                    }

                    if (alreadyProcessed)
                    {
                        continue;
                    }

                    processedTypes.push_back(currentType);

                    std::set<utils::HexCoord> visited;
                    const std::map<utils::HexCoord, model::BoardCell> &cells = board.getCells();

                    for (std::map<utils::HexCoord, model::BoardCell>::const_iterator cellIt = cells.begin(); cellIt != cells.end(); ++cellIt)
                    {
                        const utils::HexCoord &startCoord = cellIt->first;
                        const model::BoardCell &startCell = cellIt->second;

                        if (visited.count(startCoord) != 0 || !matchesConnectedType(startCell, currentType))
                        {
                            continue;
                        }

                        std::queue<utils::HexCoord> frontier;
                        frontier.push(startCoord);
                        visited.insert(startCoord);

                        std::size_t groupSize = 0;

                        while (!frontier.empty())
                        {
                            utils::HexCoord currentCoord = frontier.front();
                            frontier.pop();
                            ++groupSize;

                            std::vector<const model::BoardCell *> neighbors = board.getAdjacentCells(currentCoord);
                            for (std::size_t k = 0; k < neighbors.size(); ++k)
                            {
                                const model::BoardCell *neighbor = neighbors[k];
                                const utils::HexCoord &neighborCoord = neighbor->getCoordinate();

                                if (visited.count(neighborCoord) == 0 &&
                                    matchesConnectedType(*neighbor, currentType))
                                {
                                    visited.insert(neighborCoord);
                                    frontier.push(neighborCoord);
                                }
                            }
                        }

                        for (std::size_t j = 0; j < rules.size(); ++j)
                        {
                            if (rules[j].type == currentType && groupSizeMatchesRule(groupSize, rules[j]))
                            {
                                score += rules[j].score;
                            }
                        }
                    }
                }

                return score;
            }
        }

        std::size_t NatureSpiritScoreCalculator(const model::NatureSpiritCard &card, const model::PersonalBoard &board)
        {
            if (!card.isCubePlaced())
            {
                return 0;
            }

            const scoring::NatureSpiritEffect &effect = card.getEffect();

            if (effect.type == scoring::NSEffectType::CountConnectedGroups)
            {
                return computeConnectedGroups(effect, board);
            }

            return computeLandscapeCount(effect, board);
        }

    } // namespace scoring
} // namespace harmonies
