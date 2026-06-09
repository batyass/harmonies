#include "mainwindow.h"

#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMessageBox>
#include <QRadioButton>
#include <QVBoxLayout>

#include <algorithm>

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

bool MainWindow::chooseStartingPlayer(std::vector<std::string> &playerNames)
{
    if (playerNames.size() <= 1)
    {
        return true;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Premier joueur");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel("Quel est le dernier joueur ayant vu un paysage magnifique ?", &dialog);
    label->setWordWrap(true);
    layout->addWidget(label);

    QButtonGroup *group = new QButtonGroup(&dialog);
    group->setExclusive(true);

    for (std::size_t i = 0; i < playerNames.size(); ++i)
    {
        QRadioButton *button = new QRadioButton(QString::fromStdString(playerNames[i]), &dialog);
        group->addButton(button, static_cast<int>(i));
        layout->addWidget(button);

        if (i == 0)
        {
            button->setChecked(true);
        }
    }

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);

    if (dialog.exec() != QDialog::Accepted)
    {
        return false;
    }

    const int selectedId = group->checkedId();
    if (selectedId > 0)
    {
        std::rotate(playerNames.begin(),
                    playerNames.begin() + selectedId,
                    playerNames.end());
    }

    return true;
}

bool MainWindow::runSetupAndCreateGame()
{
    SetupDialog setup(this);
    if (setup.exec() != QDialog::Accepted)
    {
        return false;
    }

    std::vector<std::string> playerNames = setup.getPlayerNames();
    if (!chooseStartingPlayer(playerNames))
    {
        return false;
    }

    return createGameFromConfig(
        playerNames,
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
