#include <iostream>
#include <vector>
#include <string>
#include <exception>

#include "core/Game.h"
#include "ui/SetupMenu.h"
#include "ui/ConsoleInputHandler.h"
#include "ui/ConsoleRenderer.h"
#include "model/Player.h"
#include "model/GameConfig.h"

int main() {
    try {
        // 1. Configuration de la partie via le menu de setup
        harmonies::ui::SetupMenu setupMenu;
        harmonies::model::GameConfig config = setupMenu.run();
        const std::vector<std::string>& playerNames = setupMenu.getPlayerNames();

        // 2. Initialisation du moteur de jeu
        harmonies::core::Game game(config, playerNames);
        game.initGame();

        // 3. Initialisation des composants UI
        harmonies::ui::ConsoleRenderer renderer;
        harmonies::ui::ConsoleInputHandler inputHandler(game, renderer);

        // 4. Boucle de jeu principale
        while (!game.isGameOver()) {
            inputHandler.processInput();
        }

        // 5. Clôture de la partie
        std::cout << "\n========================================" << std::endl;
        std::cout << "           FIN DE LA PARTIE             " << std::endl;
        std::cout << "========================================\n" << std::endl;

        // Conversion des unique_ptr en raw pointers pour le renderer
        std::vector<harmonies::model::Player*> playersPtrs;
        for (const auto& playerPtr : game.getPlayers()) {
            playersPtrs.push_back(playerPtr.get());
        }

        renderer.displayEndGame(playersPtrs);

    } catch (const std::exception& e) {
        std::cerr << "Une erreur inattendue est survenue : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
