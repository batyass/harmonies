#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace harmonies
{
    namespace core
    {

        /**
         * @brief Machine à états finis pour le contrôleur Game.
         */
        enum class GameState
        {
            NotStarted,           // Le jeu n'a pas encore été initialisé
            WaitingForSlotChoice, // En attente que le joueur choisisse un slot sur le plateau central
            WaitingForPlacement,  // En attente que le joueur place ses jetons sur son plateau personnel
            WaitingForTurnEndChoice, // Les actions obligatoires sont terminees ; le joueur peut encore faire des actions optionnelles avant de terminer son tour
            GameOver              // La partie est terminée
        };

    }
}

#endif
