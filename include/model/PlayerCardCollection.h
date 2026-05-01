#ifndef PLAYERCARDCOLLECTION_H
#define PLAYERCARDCOLLECTION_H

#include <cstddef>
#include <vector>

#include "model/AnimalCard.h"

namespace harmonies {
namespace model {

class PlayerCardCollection {
private:
    std::vector<AnimalCard> cards;

public:
    void addCard(const AnimalCard& card);
    void removeCard(std::size_t index);
    std::size_t getCardCount() const;
    const std::vector<AnimalCard>& getCards() const;
};

}
}

#endif
