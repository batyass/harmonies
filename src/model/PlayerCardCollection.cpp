#include "model/PlayerCardCollection.h"
#include "model/AnimalCard.h"
#include <algorithm>

namespace harmonies {
namespace model {

PlayerCardCollection::PlayerCardCollection() {}

PlayerCardCollection::~PlayerCardCollection() {}

void PlayerCardCollection::addCard(AnimalCard* card) {
    if (card != nullptr) {
        cards.push_back(card);
    }
}

void PlayerCardCollection::removeCard(AnimalCard* card) {
    auto it = std::find(cards.begin(), cards.end(), card);
    if (it != cards.end()) {
        cards.erase(it);
    }
}

std::size_t PlayerCardCollection::getCardCount() const {
    return cards.size();
}

}
}