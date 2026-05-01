#include "model/NatureSpiritCard.h"

namespace harmonies {
    namespace model {

        NatureSpiritCard::NatureSpiritCard(std::string name, Pattern pattern, scoring::NatureSpiritEffect* effect)
            : name(name), pattern(pattern), cubePlaced(false), effect(effect) {}

        const std::string& NatureSpiritCard::getName() const { return name; }
        const Pattern& NatureSpiritCard::getPattern() const { return pattern; }
        scoring::NatureSpiritEffect* NatureSpiritCard::getEffect() const { return effect; }

        bool NatureSpiritCard::isCubePlaced() const { return cubePlaced; }

        bool NatureSpiritCard::placeCube() {
            if (cubePlaced) return false;
            cubePlaced = true;
            return true;
        }

    } // namespace model
} // namespace harmonies
