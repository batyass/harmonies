#ifndef NATURESPIRITDECK_H
#define NATURESPIRITDECK_H

#include "model/NatureSpiritCard.h"
#include <cstddef>
#include <vector>

namespace harmonies
{
    namespace model
    {

        class NatureSpiritDeck
        {
        private:
            std::vector<NatureSpiritCard> cards;

        public:
            // allCards : toutes les cartes esprit, melangees dans le constructeur
            NatureSpiritDeck(std::vector<NatureSpiritCard> allCards);

            // Distribution initiale : retourne 2 cartes par joueur
            // Precondition : cards.size() >= 2 * numPlayers
            std::vector<std::vector<NatureSpiritCard>> deal(std::size_t numPlayers);
        };

    } // namespace model
} // namespace harmonies

#endif
