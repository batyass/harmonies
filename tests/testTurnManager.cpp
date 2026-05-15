#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "core/TurnManager.h"
#include "model/Player.h"
#include "model/PersonalBoard.h"

using harmonies::core::TurnManager;
using harmonies::model::Player;
using harmonies::model::BoardSide;

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

    Player* p1 = new Player("Alice", static_cast<BoardSide>(0));
    Player* p2 = new Player("Bob", static_cast<BoardSide>(0));
    Player* p3 = new Player("Charlie", static_cast<BoardSide>(0));

    std::vector<Player*> playerList = {p1, p2, p3};

    TurnManager manager(playerList);

    check(manager.getCurrentPlayer() == p1, "Le premier joueur doit etre Alice (index 0)", failures);
    check(manager.getTurnCount() == 1, "Le jeu doit commencer au tour 1", failures);
    check(manager.isNewCycle() == true, "Au debut, c'est un nouveau cycle", failures);

    manager.nextTurn();
    check(manager.getCurrentPlayer() == p2, "Apres nextTurn(), le joueur actif doit etre Bob (index 1)", failures);
    check(manager.getTurnCount() == 1, "Le compteur de tour doit toujours etre a 1", failures);
    check(manager.isNewCycle() == false, "Au milieu du tour, ce n'est pas un nouveau cycle", failures);

    manager.nextTurn();
    check(manager.getCurrentPlayer() == p3, "Apres le deuxieme nextTurn(), le joueur doit etre Charlie (index 2)", failures);

    manager.nextTurn();
    check(manager.getCurrentPlayer() == p1, "Apres le tour de Charlie, on doit revenir a Alice (boucle)", failures);
    check(manager.getTurnCount() == 2, "Le compteur de tour global doit passer a 2", failures);
    check(manager.isNewCycle() == true, "Apres la boucle, c'est a nouveau un nouveau cycle", failures);

    try {
        std::vector<Player*> emptyList;
        TurnManager failManager(emptyList);
        check(false, "Le constructeur devrait lever une exception pour une liste vide", failures);
    } catch (const std::invalid_argument&) {
        check(true, "Le constructeur a bien lever une exception pour une liste vide", failures);
    }

    delete p1;
    delete p2;
    delete p3;

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}