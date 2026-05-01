#pragma once

/*Contient les paramètres de la partie :
solo ou multi
nombre de joueurs
face du plateau personnel A/B
esprit de la nature activé ou non
Les règles distinguent bien multi/solo, face A/B, et option Esprit de la nature.
*/

#include <cstddef>
#include <string>
#include <stdexcept>

#include "model/BoardSide.h"

namespace harmonies
{
    namespace model
    {
        class GameConfig
        {
        private:
            std::size_t nbPlayer;
            BoardSide side;
            bool natureSpiritOption;

        public:
            GameConfig(std::size_t nb, BoardSide s, bool o);
            std::size_t getNbPlayer() const { return nbPlayer; }
            std::string getMode() const;
            BoardSide getSide() const { return side; }
            bool getNatureSpiritOption() const { return natureSpiritOption; }
        };

    }
}
