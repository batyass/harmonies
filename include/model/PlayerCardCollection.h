#ifndef PLAYERCARDCOLLECTION_H
#define PLAYERCARDCOLLECTION_H

#include <vector>
#include <cstddef>

namespace harmonies {
namespace model {

class AnimalCard; 

class PlayerCardCollection {
private:
    std::vector<AnimalCard*> cards;

public:
    PlayerCardCollection();
    ~PlayerCardCollection();

    void addCard(AnimalCard* card);
    void removeCard(AnimalCard* card);
    std::size_t getCardCount() const;
};

}
}

#endif