#include "mainwindow.h"

#include <QMessageBox>

#include "core/Game.h"
#include "gamestagewidget.h"
#include "model/GameConfig.h"
#include "setupdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), game(), stageWidget(nullptr)
{
    this->setWindowTitle("Harmonies - Jalon Qt (Projet LO21)");
    this->resize(1300, 850);

    if (!runSetupAndCreateGame())
    {
        close();
        return;
    }

    showGameStage();
}

bool MainWindow::isReady() const
{
    return game != nullptr && stageWidget != nullptr;
}

bool MainWindow::runSetupAndCreateGame()
{
    SetupDialog setup(this);
    if (setup.exec() != QDialog::Accepted)
    {
        return false;
    }

    try
    {
        std::vector<std::string> names = setup.getPlayerNames();
        harmonies::model::GameConfig config(
            names.size(),
            setup.getBoardSide(),
            setup.isNatureSpiritEnabled());

        std::unique_ptr<harmonies::core::Game> newGame(
            new harmonies::core::Game(config, names));
        newGame->initGame();
        game = std::move(newGame);
        return true;
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Erreur de lancement", QString::fromUtf8(e.what()));
    }
    catch (...)
    {
        QMessageBox::critical(this, "Erreur de lancement", "Une erreur inattendue est survenue lors du demarrage de la partie.");
    }

    return false;
}

void MainWindow::showGameStage()
{
    if (!game)
    {
        return;
    }

    GameStageWidget *newStage = new GameStageWidget(game.get(), this);
    connect(newStage, &GameStageWidget::requestNewGame, this, &MainWindow::onRequestNewGame);
    connect(newStage, &GameStageWidget::requestReturnToMenu, this, &MainWindow::onRequestReturnToMenu);

    setCentralWidget(newStage);
    stageWidget = newStage;
}

void MainWindow::onRequestNewGame()
{
    if (!runSetupAndCreateGame())
    {
        return;
    }

    showGameStage();
}

void MainWindow::onRequestReturnToMenu()
{
    game.reset();

    if (!runSetupAndCreateGame())
    {
        close();
        return;
    }

    showGameStage();
}
