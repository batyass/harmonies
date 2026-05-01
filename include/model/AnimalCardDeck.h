#ifndef ANIMALCARDDECK_H
#define ANIMALCARDDECK_H

#include "model/AnimalCard.h"
#include <vector>

namespace harmonies
{
    namespace model
    {

        class AnimalCardDeck
        {
        private:
            std::vector<AnimalCard> drawPile; // cartes non revelees, melangees
            std::vector<AnimalCard> visible;  // cartes visibles au centre
            size_t visibleSlots;              // nombre max de cartes visibles (3 solo / 5 multi)

        public:
            // allCards : toutes les cartes du jeu, melangees dans le constructeur
            // visibleSlots : 3 en solo, 5 en multi
            AnimalCardDeck(std::vector<AnimalCard> allCards, size_t visibleSlots);

            // Remplit les emplacements vides depuis la pioche (appele en fin de tour)
            void refill();

            // Prend la carte visible a l'index donne et la retire des visibles
            AnimalCard takeVisible(size_t index);

            const std::vector<AnimalCard> &getVisible() const;
            bool drawPileEmpty() const;
            size_t visibleCount() const;
        };

    } // namespace model
} // namespace harmonies

#endif