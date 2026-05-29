#include "ui/ConsoleInputHandler.h"
#include "utils/HexCoord.h"
#include "model/TokenType.h"
#include <iostream>
#include <limits>

namespace harmonies {
namespace ui {

ConsoleInputHandler::ConsoleInputHandler(core::Game& gameInstance, const ConsoleRenderer& rendererInstance)
    : game(gameInstance), renderer(rendererInstance) {}

void ConsoleInputHandler::processInput() {
    core::GameState state = game.getState();

    if (state == core::GameState::WaitingForSlotChoice) {
        int slotIndexRaw;
        while (true) {
            // Intégration du Renderer de Néo avant la saisie
            renderer.displayCentralBoard(*game.getCentralBoard());
            renderer.displayPersonalBoard(*game.getCurrentPlayer()->getBoard());

            std::cout << "Choisissez un slot du marche : ";
            if (std::cin >> slotIndexRaw) {
                if (slotIndexRaw < 0) {
                    std::cout << "Valeur negative invalide, veuillez entrer un nombre positif.\n";
                    continue;
                }
                if (game.takeTokensFromSlot(static_cast<std::size_t>(slotIndexRaw))) {
                    break;
                } else {
                    std::cout << "Choix refuse par le jeu.\n";
                }
            } else {
                if (std::cin.eof()) {
                    std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                    return;
                }
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Entree invalide, veuillez entrer un nombre.\n";
            }
        }
    } else if (state == core::GameState::WaitingForPlacement) {
        int q, r;
        int tokenTypeInt;
        while (true) {
            // Affichage du plateau personnel mis à jour pour le placement
            renderer.displayPersonalBoard(*game.getCurrentPlayer()->getBoard());

            std::cout << "Entrez les coordonnees q et r et le type de jeton (0-5) : ";
            if (std::cin >> q >> r >> tokenTypeInt) {
                if (tokenTypeInt < 0 || tokenTypeInt > 5) {
                    std::cout << "Type de jeton invalide (valeur attendue : 0 a 5).\n";
                    continue;
                }
                utils::HexCoord coord(q, r);
                if (game.placeTokenOnBoard(coord, static_cast<model::TokenType>(tokenTypeInt))) {
                    break;
                } else {
                    std::cout << "Placement refuse par le jeu.\n";
                }
            } else {
                if (std::cin.eof()) {
                    std::cout << "\nFin de flux (EOF) detectee. Abandon de la saisie.\n";
                    return;
                }
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Entree invalide, veuillez entrer trois nombres entiers.\n";
            }
        }
    } else if (state == core::GameState::GameOver) {
        std::cout << "La partie est terminee.\n";
    }
}

}
}
