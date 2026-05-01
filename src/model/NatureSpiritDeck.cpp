#include "model/NatureSpiritDeck.h"

#include <algorithm>
#include <random>

namespace harmonies {
    namespace model {

        NatureSpiritDeck::NatureSpiritDeck(std::vector<NatureSpiritCard> allCards)
            : cards(std::move(allCards)) {
            std::mt19937 randomEngine(std::random_device{}());
            std::shuffle(cards.begin(), cards.end(), randomEngine);
        }

        std::vector<std::vector<NatureSpiritCard>> NatureSpiritDeck::deal(int numPlayers) {
            std::vector<std::vector<NatureSpiritCard>> hands;
            for (int i = 0; i < numPlayers; ++i) {
                std::vector<NatureSpiritCard> hand;
                hand.push_back(cards.back());
                cards.pop_back();
                hand.push_back(cards.back());
                cards.pop_back();
                hands.push_back(hand);
            }
            return hands; //retourne toutes les mains, faudra distribuer ailleurs quand Player sera fait
        }



    } // namespace model
} // namespace harmonies
