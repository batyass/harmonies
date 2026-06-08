#include "mainwindow.h"

#include <QMessageBox>

#include "core/Game.h"
#include "gamestagewidget.h"
#include "model/GameConfig.h"
#include "setupdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      game(),
      stageWidget(nullptr),
      lastPlayerNames(),
      lastBoardSide(harmonies::model::BoardSide::A),
      lastNatureSpiritEnabled(false),
      hasLastSetup(false)
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

bool MainWindow::createGameFromConfig(const std::vector<std::string> &playerNames,
                                      harmonies::model::BoardSide boardSide,
                                      bool natureSpiritEnabled)
{
    try
    {
        harmonies::model::GameConfig config(
            playerNames.size(),
            boardSide,
            natureSpiritEnabled);

        std::unique_ptr<harmonies::core::Game> newGame(
            new harmonies::core::Game(config, playerNames));
        newGame->initGame();
        game = std::move(newGame);

        lastPlayerNames = playerNames;
        lastBoardSide = boardSide;
        lastNatureSpiritEnabled = natureSpiritEnabled;
        hasLastSetup = true;

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

bool MainWindow::runSetupAndCreateGame()
{
    SetupDialog setup(this);
    if (setup.exec() != QDialog::Accepted)
    {
        return false;
    }

    return createGameFromConfig(
        setup.getPlayerNames(),
        setup.getBoardSide(),
        setup.isNatureSpiritEnabled());
}

bool MainWindow::restartWithLastSetup()
{
    if (!hasLastSetup)
    {
        return runSetupAndCreateGame();
    }

    return createGameFromConfig(lastPlayerNames, lastBoardSide, lastNatureSpiritEnabled);
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
    if (!restartWithLastSetup())
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
