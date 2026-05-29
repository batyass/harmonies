#ifndef SETUPMENU_H
#define SETUPMENU_H

#include <string>
#include <vector>
#include "model/GameConfig.h"

namespace harmonies
{

    namespace ui
    {

        class SetupMenu
        {
            private:
                std::vector<std::string> playerNames;

                static int         readInt(const std::string &prompt, int min, int max);
                static char        readChar(const std::string &prompt, char opt1, char opt2);
                static std::string readLine(const std::string &prompt);

            public:
                model::GameConfig run();

                const std::vector<std::string> &getPlayerNames() const;
        };

    } // namespace ui
} // namespace harmonies

#endif
