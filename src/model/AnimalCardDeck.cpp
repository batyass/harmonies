#include "model/AnimalCardDeck.h"

#include <algorithm>
#include <random>
#include <stdexcept>

namespace harmonies
{
    namespace model
    {

        AnimalCardDeck::AnimalCardDeck(std::vector<AnimalCard> allCards, size_t visibleSlots)
            : visibleSlots(visibleSlots)
        {
            if (visibleSlots != 3 && visibleSlots != 5)
            { // verification de la valeur de visibleSlots
                throw std::out_of_range("Value of visibleSlots out of range\n");
            }
            std::mt19937 randomEngine(std::random_device{}());
            std::shuffle(allCards.begin(), allCards.end(), randomEngine);
            drawPile = std::move(allCards);
            refill();
        }

        void AnimalCardDeck::refill()
        {
            while (visible.size() < visibleSlots && !drawPile.empty())
            {
                visible.push_back(drawPile.back());
                drawPile.pop_back();
            }
        }

        AnimalCard AnimalCardDeck::takeVisible(size_t index)
        {
            if (index >= visible.size())
            { // verification de la valeur de index
                throw std::out_of_range("Value of index out of range\n");
            }
            AnimalCard card = visible[index];
            visible.erase(visible.begin() + index);
            return card;
        }

        const std::vector<AnimalCard> &AnimalCardDeck::getVisible() const { return visible; }
        bool AnimalCardDeck::drawPileEmpty() const { return drawPile.empty(); }
        size_t AnimalCardDeck::visibleCount() const { return visible.size(); }

    } // namespace model
} // namespace harmonies