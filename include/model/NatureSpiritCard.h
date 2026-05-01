#ifndef NATURESPIRITCARD_H
#define NATURESPIRITCARD_H

#include "model/Pattern.h"
#include "scoring/NatureSpiritEffect.h"
#include <string>

namespace harmonies {

    namespace model {

        class NatureSpiritCard {
            private:
                std::string name;
                Pattern pattern;
                bool cubePlaced;                        // false = cube encore sur la carte
                scoring::NatureSpiritEffect* effect;    // regle de score speciale
                //J'ai vu qu'il y avait un fichier NatureSpiritEffect.h donc j'ai mis ca

            public:
                
                NatureSpiritCard(std::string name, Pattern pattern, scoring::NatureSpiritEffect* effect);

                const std::string& getName() const;
                const Pattern& getPattern() const;
                scoring::NatureSpiritEffect* getEffect() const;

                bool isCubePlaced() const;

                // Appelé par NatureSpiritPlacementService apres confirmation du pattern
                // Retourne false si le cube est deja pose
                bool placeCube();
        };

    } // namespace model
} // namespace harmonies

#endif
