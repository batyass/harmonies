#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "core/TurnManager.h"
#include "model/Player.h"

using harmonies::core::TurnManager;
using harmonies::model::BoardSide;
using harmonies::model::Player;

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

    Player p1("Alice", BoardSide::A);
    Player p2("Bob", BoardSide::A);
    Player p3("Charlie", BoardSide::A);

    std::vector<Player *> playerList = {&p1, &p2, &p3};

    TurnManager manager(playerList);

    check(manager.getCurrentPlayer() == &p1, "Le premier joueur doit etre Alice (index 0)", failures);
    check(manager.getTurnCount() == 1, "Le jeu doit commencer au tour 1", failures);
    check(manager.isNewTurn() == true, "Au debut, on doit etre au premier joueur du tour", failures);

    manager.nextTurn();
    check(manager.getCurrentPlayer() == &p2, "Apres nextTurn(), le joueur actif doit etre Bob (index 1)", failures);
    check(manager.getTurnCount() == 1, "Le compteur de tour doit toujours etre a 1", failures);
    check(manager.isNewTurn() == false, "Au milieu du tour, on ne doit pas etre au debut d'un nouveau tour", failures);

    manager.nextTurn();
    check(manager.getCurrentPlayer() == &p3, "Apres le deuxieme nextTurn(), le joueur doit etre Charlie (index 2)", failures);

    manager.nextTurn();
    check(manager.getCurrentPlayer() == &p1, "Apres le tour de Charlie, on doit revenir a Alice (boucle)", failures);
    check(manager.getTurnCount() == 2, "Le compteur de tour global doit passer a 2", failures);
    check(manager.isNewTurn() == true, "Apres la boucle, on doit revenir au debut d'un nouveau tour", failures);

    try
    {
        std::vector<Player *> emptyList;
        TurnManager failManager(emptyList);
        check(false, "Le constructeur devrait lever une exception pour une liste vide", failures);
    }
    catch (const std::invalid_argument &)
    {
        check(true, "Le constructeur a bien lever une exception pour une liste vide", failures);
    }

    try
    {
        std::vector<Player *> invalidList = {&p1, nullptr};
        TurnManager failManager(invalidList);
        check(false, "Le constructeur devrait lever une exception si la liste contient nullptr", failures);
    }
    catch (const std::invalid_argument &)
    {
        check(true, "Le constructeur a bien lever une exception pour un joueur nullptr", failures);
    }

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
