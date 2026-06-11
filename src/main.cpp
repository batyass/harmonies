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
#include "scoring/SoloScoreEvaluator.h"

int main() {
    try {
        harmonies::ui::SetupMenu setupMenu;
        harmonies::model::GameConfig config = setupMenu.run();
        const std::vector<std::string>& playerNames = setupMenu.getPlayerNames();

        harmonies::core::Game game(config, playerNames);
        game.initGame();

        harmonies::ui::ConsoleRenderer renderer;
        harmonies::ui::ConsoleInputHandler inputHandler(game, renderer);

        // Boucle de jeu principale
        while (!game.isGameOver()) {
            inputHandler.processInput();
            if (!std::cin.good())
            {
                break;
            }
        }

        std::cout << "\n========================================" << std::endl;
        std::cout << "           FIN DE LA PARTIE             " << std::endl;
        std::cout << "========================================\n" << std::endl;

        std::vector<harmonies::model::Player*> playersPtrs;
        for (const auto& playerPtr : game.getPlayers()) {
            playersPtrs.push_back(playerPtr.get());
        }

        renderer.displayEndGame(playersPtrs);

        if (config.getNbPlayer() == 1 && !playersPtrs.empty())
        {
            const harmonies::model::Player *soloPlayer = playersPtrs[0];
            const std::size_t suns = harmonies::scoring::evaluateSoloScore(
                static_cast<std::size_t>(soloPlayer->getScore()),
                soloPlayer->getBoard()->getSide(),
                config.getNatureSpiritOption());

            std::cout << "\nEvaluation solo : " << soloPlayer->getScore()
                      << " points = " << suns << " soleil(s)\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Une erreur inattendue est survenue : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
