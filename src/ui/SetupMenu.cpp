#include "ui/SetupMenu.h"
#include <cctype>
#include <iostream>
#include <limits>

namespace harmonies
{
    namespace ui
    {

        int SetupMenu::readInt(const std::string &prompt, int min, int max)
        {
            int value;
            while (true)
            {
                std::cout << prompt;
                if (std::cin >> value && value >= min && value <= max)
                {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    return value;
                }
                std::cout << "  Please enter a number between " << min << " and " << max << ".\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        char SetupMenu::readChar(const std::string &prompt, char opt1, char opt2)
        {
            char a = static_cast<char>(std::tolower(static_cast<unsigned char>(opt1)));
            char b = static_cast<char>(std::tolower(static_cast<unsigned char>(opt2)));
            char c;
            while (true)
            {
                std::cout << prompt;
                if (std::cin >> c)
                {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                    if (c == a || c == b) return c;
                }
                else
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                std::cout << "  Please enter '" << opt1 << "' or '" << opt2 << "'.\n";
            }
        }

        std::string SetupMenu::readLine(const std::string &prompt)
        {
            std::string line;
            while (true)
            {
                std::cout << prompt;
                if (std::getline(std::cin, line) && !line.empty())
                    return line;
                std::cout << "  Name cannot be empty.\n";
            }
        }

        model::GameConfig SetupMenu::run()
        {
            std::cout << "=== Harmonies - Game Setup ===\n\n";

            int nb = readInt("Number of players (1-4): ", 1, 4);

            model::BoardSide side = (readChar("Board side (A/B): ", 'a', 'b') == 'a')
                ? model::BoardSide::A : model::BoardSide::B;

            bool spirit = false;
            if (nb > 1)
                spirit = (readChar("Enable Nature Spirit option? (y/n): ", 'y', 'n') == 'y');

            std::cout << '\n';
            playerNames.clear();
            for (int i = 0; i < nb; i++)
            {
                std::string name;
                while (true)
                {
                    name = readLine("Player " + std::to_string(i+1) + " name: ");
                    bool duplicate = false;
                    for (const std::string &existing : playerNames)
                    {
                        if (existing == name)
                        {
                            duplicate = true;
                            break;
                        }
                    }
                    if (!duplicate) break;
                    std::cout << "  Name '" << name << "' is already taken. Choose a different name.\n";
                }
                playerNames.push_back(name);
            }

            std::cout << "\n--- Setup complete ---\n";
            std::cout << "Players : " << nb << '\n';
            std::cout << "Board   : Side " << (side == model::BoardSide::A ? 'A' : 'B') << '\n';
            std::cout << "Spirit  : " << (spirit ? "enabled" : "disabled") << '\n';
            std::cout << "Names   : ";
            for (std::size_t i = 1; i < playerNames.size(); i++)
            {
                if (i > 0) std::cout << ", ";
                std::cout << playerNames[i];
            }
            std::cout << "\n\n";

            return model::GameConfig(static_cast<std::size_t>(nb), side, spirit);
        }

        const std::vector<std::string> &SetupMenu::getPlayerNames() const
        {
            return playerNames;
        }

    } // namespace ui
} // namespace harmonies
