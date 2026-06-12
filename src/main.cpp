#include <QApplication>
#include "core/Game.h"
#include "ui/VuePartie.h"
#include "model/GameConfig.h"
#include <vector>
#include <string>
#include <iostream>

int main(int argc, char *argv[]) {
    try {
        QApplication app(argc, argv);

        // Configuration factice pour tester l'interface visuelle directement
        harmonies::model::GameConfig config(2, harmonies::model::BoardSide::A, true);
        std::vector<std::string> playerNames = {"Yassir", "Peilin"};
        
        harmonies::core::Game game(config, playerNames);
        
        // On initialise le jeu pour éviter le crash du marché vide (en attendant le fix de Clément)
        game.initGame();

        // Lancement de ton interface graphique
        harmonies::ui::VuePartie fenetre(game);
        fenetre.show();

        return app.exec();
    } catch (const std::exception& e) {
        std::cerr << "Une erreur inattendue est survenue : " << e.what() << std::endl;
        return 1;
    }
}
