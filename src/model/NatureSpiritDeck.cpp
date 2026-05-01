#include "model/NatureSpiritDeck.h"

#include <algorithm>
#include <random>
#include <stdexcept>

namespace harmonies
{
    namespace model
    {

        NatureSpiritDeck::NatureSpiritDeck(std::vector<NatureSpiritCard> allCards)
            : cards(std::move(allCards))
        {
            std::mt19937 randomEngine(std::random_device{}());
            std::shuffle(cards.begin(), cards.end(), randomEngine);
        }

        std::vector<std::vector<NatureSpiritCard>> NatureSpiritDeck::deal(std::size_t numPlayers)
        {
            if (numPlayers == 0)
            {
                throw std::out_of_range("Number of players must be positive");
            }

            if (cards.size() < 2 * numPlayers)
            {
                throw std::out_of_range("Not enough nature spirit cards to deal");
            }

            std::vector<std::vector<NatureSpiritCard>> hands;
            hands.reserve(numPlayers);

            for (std::size_t i = 0; i < numPlayers; ++i)
            {
                std::vector<NatureSpiritCard> hand;
                hand.reserve(2);
                hand.push_back(cards.back());
                cards.pop_back();
                hand.push_back(cards.back());
                cards.pop_back();
                hands.push_back(std::move(hand));
            }
            return hands; // retourne toutes les mains, faudra distribuer ailleurs quand Player sera fait
        }

    } // namespace model
} // namespace harmonies
