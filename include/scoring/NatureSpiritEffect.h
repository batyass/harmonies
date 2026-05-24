#ifndef NATURESPIRITEFFECT_H
#define NATURESPIRITEFFECT_H

#include <cstddef>
#include <vector>

#include "model/TokenType.h"

namespace harmonies
{
    namespace scoring
    {
        enum class NSEffectType
        {
            CountLandscapes,      // Score based on the number of matching landscape stacks
            CountConnectedGroups, // Score based on the size of connected landscape groups
        };

        struct LandscapeCountRule
        {
            model::TokenType type;
            std::size_t requiredHeight; // 0 means any height
            std::size_t score;
        };

        struct ConnectedGroupRule
        {
            model::TokenType type;
            std::size_t minGroupSize;
            std::size_t maxGroupSize; // 0 means no upper bound
            std::size_t score;
        };

        struct NatureSpiritEffect
        {
            NSEffectType type;
            std::vector<LandscapeCountRule> landscapeRules;
            std::vector<ConnectedGroupRule> connectedRules;

            NatureSpiritEffect() : type(NSEffectType::CountLandscapes) {}
        };
    }

} // namespace harmonies

#endif
