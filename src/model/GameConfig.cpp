#include "model/GameConfig.h"

namespace harmonies
{
    namespace model
    {
        GameConfig::GameConfig(std::size_t nb, BoardSide s, bool o) : nbPlayer(nb), side(s), natureSpiritOption(o)
        {
            if (nb > 4 || nb == 0)
            {
                throw std::invalid_argument("nbPlayer must be between 1 and 4");
            }
        }
        std::string GameConfig::getMode() const
        {
            if (nbPlayer == 1)
            {
                return "solo";
            }
            return "multi";
        }
    }
}
