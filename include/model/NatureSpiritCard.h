#ifndef NATURESPIRITCARD_H
#define NATURESPIRITCARD_H

#include "model/Pattern.h"
#include "scoring/NatureSpiritEffect.h"
#include <string>

namespace harmonies
{

    namespace model
    {

        class NatureSpiritCard
        {
        private:
            std::string name;
            Pattern pattern;
            bool cubePlaced; // false = cube encore sur la carte
            const scoring::NatureSpiritEffect *effect; // effet non possede par la carte

        public:
            NatureSpiritCard(std::string name, Pattern pattern, const scoring::NatureSpiritEffect *effect);

            const std::string &getName() const;
            const Pattern &getPattern() const;
            const scoring::NatureSpiritEffect *getEffect() const;

            bool isCubePlaced() const;

            // Appelé par NatureSpiritPlacementService apres confirmation du pattern
            // Retourne false si le cube est deja pose
            bool placeCube();
        };

    } // namespace model
} // namespace harmonies

#endif
