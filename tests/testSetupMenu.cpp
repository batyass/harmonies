#include <iostream>
#include "ui/SetupMenu.h"

using harmonies::model::GameConfig;
using harmonies::ui::SetupMenu;

int main()
{
    SetupMenu menu;
    GameConfig config = menu.run();

    std::cout << "=== Parsed result ===\n";
    std::cout << "nbPlayer : " << config.getNbPlayer() << '\n';
    std::cout << "mode     : " << config.getMode() << '\n';
    std::cout << "side     : " << (config.getSide() == harmonies::model::BoardSide::A ? 'A' : 'B') << '\n';
    std::cout << "spirit   : " << (config.getNatureSpiritOption() ? "yes" : "no") << '\n';
    std::cout << "names    : ";
    for (const std::string &name : menu.getPlayerNames())
        std::cout << '[' << name << "] ";
    std::cout << '\n';

    return 0;
}
