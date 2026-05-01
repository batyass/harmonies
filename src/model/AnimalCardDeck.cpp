#include "model/AnimalCardDeck.h"

#include <algorithm>
#include <random>

namespace harmonies {
    namespace model {

        AnimalCardDeck::AnimalCardDeck(std::vector<AnimalCard> allCards, int visibleSlots)
            : visibleSlots(visibleSlots) {
            std::mt19937 randomEngine(std::random_device{}());
            std::shuffle(allCards.begin(), allCards.end(), randomEngine);
            drawPile = std::move(allCards);
            refill();
        }

        void AnimalCardDeck::refill() {
            while (static_cast<int>(visible.size()) < visibleSlots && !drawPile.empty()) {
                visible.push_back(drawPile.back());
                drawPile.pop_back();
            }
        }

        AnimalCard AnimalCardDeck::takeVisible(int index) {
            AnimalCard card = visible[index];
            visible.erase(visible.begin() + index);
            return card;
        }

        const std::vector<AnimalCard>& AnimalCardDeck::getVisible() const {return visible;}
        bool AnimalCardDeck::drawPileEmpty() const {return drawPile.empty();}
        int  AnimalCardDeck::visibleCount() const {return visible.size();}

    } // namespace model
} // namespace harmonies