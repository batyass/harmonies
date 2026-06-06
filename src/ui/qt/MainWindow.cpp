#include "ui/qt/MainWindow.h"

#include <QLabel>
#include <QPushButton>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>

#include "ui/qt/GameControllerQt.h"

namespace harmonies
{
    namespace ui
    {
        MainWindow::MainWindow(QWidget *parent)
            : QMainWindow(parent),
              controller(new GameControllerQt(this)),
              titleLabel(new QLabel("Harmonies - Version Qt Widgets (base minimale)", this)),
              statusLabel(new QLabel("Aucune partie en cours.", this)),
              newGameButton(new QPushButton("Nouvelle partie de demonstration", this))
        {
            QWidget *central = new QWidget(this);
            QVBoxLayout *layout = new QVBoxLayout(central);

            layout->addWidget(titleLabel);
            layout->addWidget(statusLabel);
            layout->addWidget(newGameButton);
            layout->addStretch();

            setCentralWidget(central);
            setWindowTitle("Harmonies Qt");
            resize(640, 240);

            connect(newGameButton, &QPushButton::clicked, controller, &GameControllerQt::startDemoGame);
            connect(controller, &GameControllerQt::gameStateChanged, this, [this]() { refreshUi(); });
            connect(controller, SIGNAL(actionFailed(QString)), statusBar(), SLOT(showMessage(QString)));

            refreshUi();
        }

        void MainWindow::refreshUi()
        {
            core::Game *game = controller->getGame();
            if (game == 0)
            {
                statusLabel->setText("Aucune partie en cours.");
                return;
            }

            QString stateText;
            switch (game->getState())
            {
            case core::GameState::NotStarted:
                stateText = "NotStarted";
                break;
            case core::GameState::WaitingForSlotChoice:
                stateText = "WaitingForSlotChoice";
                break;
            case core::GameState::WaitingForPlacement:
                stateText = "WaitingForPlacement";
                break;
            case core::GameState::WaitingForTurnEndChoice:
                stateText = "WaitingForTurnEndChoice";
                break;
            case core::GameState::GameOver:
                stateText = "GameOver";
                break;
            }

            QString playerName = game->getCurrentPlayer() != 0
                                     ? QString::fromStdString(game->getCurrentPlayer()->getName())
                                     : QString("aucun");

            statusLabel->setText(QString("Joueur courant : %1 | Etat : %2")
                                     .arg(playerName)
                                     .arg(stateText));
        }
    }
}
