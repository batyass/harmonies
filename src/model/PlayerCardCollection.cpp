#include "model/PlayerCardCollection.h"

namespace harmonies {
namespace model {

void PlayerCardCollection::addCard(const AnimalCard& card) {
    cards.push_back(card);
}

void PlayerCardCollection::removeCard(std::size_t index) {
    if (index < cards.size()) {
        cards.erase(cards.begin() + index);
    }
}

std::size_t PlayerCardCollection::getCardCount() const {
    return cards.size();
}

const std::vector<AnimalCard>& PlayerCardCollection::getCards() const {
    return cards;
}

}
}
