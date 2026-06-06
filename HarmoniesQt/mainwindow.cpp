#include "mainwindow.h"
#include "gamestagewidget.h" // Include it here safely
#include "core/Game.h"

MainWindow::MainWindow(harmonies::core::Game *backendGame, QWidget *parent)
    : QMainWindow(parent), game(backendGame)
{
    this->setWindowTitle("Harmonies - Jalon Qt (Projet LO21)");
    this->resize(1300, 850);

    stageWidget = new GameStageWidget(game, this);
    this->setCentralWidget(stageWidget);
}
