#include "mainwindow.h"
#include "setupdialog.h"
#include <QApplication>
#include <QMessageBox>
#include "model/GameConfig.h"
#include "model/BoardSide.h"
#include "core/Game.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Pop up the setup menu dialog first
    SetupDialog setup;

    if (setup.exec() == QDialog::Accepted) {
        try {
            // 2. Extrapolate dynamic vector of strings from UI input fields
            std::vector<std::string> names = setup.getPlayerNames();

            // 3. Construct your exact GameConfig object
            harmonies::model::GameConfig config(
                names.size(),
                setup.getBoardSide(),
                setup.isNatureSpiritEnabled()
                );

            // 4. Instantiate your real backend Game engine via standard RAII allocation
            harmonies::core::Game realGame(config, names);
            realGame.initGame(); // Initialize the token bag, deck, boards automatically

            // 5. Inject the pointer of your active backend engine directly into the Qt Window
            MainWindow w(&realGame);
            w.show();

            return a.exec(); // Start UI interaction loop
        } catch (const std::exception &e) {
            QMessageBox::critical(nullptr, "Erreur de lancement", QString::fromUtf8(e.what()));
            return 1;
        } catch (...) {
            QMessageBox::critical(nullptr, "Erreur de lancement", "Une erreur inattendue est survenue lors du demarrage de la partie.");
            return 1;
        }
    }

    return 0;
}
