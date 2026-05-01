#include <iostream>
#include <string>

#include "model/Player.h"
#include "model/PlayerCardCollection.h"

using harmonies::model::AnimalCard;
using harmonies::model::Player;
using harmonies::model::PlayerCardCollection;

namespace
{
    void check(bool condition, const std::string &message, int &failures)
    {
        if (condition)
        {
            std::cout << "[PASS] " << message << '\n';
        }
        else
        {
            std::cout << "[FAIL] " << message << '\n';
            ++failures;
        }
    }
}

int main()
{
    int failures = 0;

    Player player("Yassir", static_cast<harmonies::model::BoardSide>(0));
    check(player.getName() == "Yassir", "Le nom du joueur doit etre Yassir", failures);
    check(player.getScore() == 0, "Le score initial doit etre 0", failures);
    check(player.getBoard() != nullptr, "Le plateau personnel doit etre cree", failures);

    player.addPoints(15);
    check(player.getScore() == 15, "Le score doit etre de 15", failures);

    PlayerCardCollection collection;
    check(collection.getCardCount() == 0, "La collection doit etre vide au depart", failures);

    AnimalCard *dummyCard1 = reinterpret_cast<AnimalCard *>(1);
    AnimalCard *dummyCard2 = reinterpret_cast<AnimalCard *>(2);

    collection.addCard(dummyCard1);
    collection.addCard(dummyCard2);
    check(collection.getCardCount() == 2, "La collection doit avoir 2 cartes", failures);

    collection.removeCard(dummyCard1);
    check(collection.getCardCount() == 1, "La collection doit avoir 1 carte apres suppression", failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}