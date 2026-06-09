#include "gamestagewidget.h"
#include "personalboardwidget.h"
#include "centralboardwidget.h"
#include "cardmarketwidget.h"
#include "playerownedcardswidget.h"
#include "playerinfoswidget.h"
#include "spiritcardwidget.h"
#include "endgamedialog.h"
#include "core/Game.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

GameStageWidget::GameStageWidget(harmonies::core::Game *backendGame, QWidget *parent)
    : QWidget(parent), game(backendGame)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(6, 6, 6, 6);
    mainLayout->setSpacing(8);

    QVBoxLayout *leftColumn = new QVBoxLayout();
    leftColumn->setSpacing(8);

    personalBoard = new PersonalBoardWidget(game, this);
    leftColumn->addWidget(personalBoard, 5);

    spiritCard = new SpiritCardWidget(game, this);
    leftColumn->addWidget(spiritCard, 2);

    mainLayout->addLayout(leftColumn, 4);

    centralBoard = new CentralBoardWidget(game, this);
    mainLayout->addWidget(centralBoard, 4);

    QVBoxLayout *rightColumn = new QVBoxLayout();
    rightColumn->setSpacing(8);

    cardMarket = new CardMarketWidget(game, this);
    rightColumn->addWidget(cardMarket, 6);

    ownedCards = new PlayerOwnedCardsWidget(game, this);
    rightColumn->addWidget(ownedCards, 4);

    playerInfos = new PlayerInfosWidget(game, this);
    rightColumn->addWidget(playerInfos, 4);

    personalBoard->setPlayerInfosWidget(playerInfos);

    mainLayout->addLayout(rightColumn, 3);

    connect(centralBoard, &CentralBoardWidget::slotSelected, this, &GameStageWidget::refreshAllComponents);
    connect(personalBoard, &PersonalBoardWidget::boardUpdated, this, &GameStageWidget::refreshAllComponents);
    connect(cardMarket, &CardMarketWidget::marketUpdated, this, &GameStageWidget::refreshAllComponents);
    connect(spiritCard, &SpiritCardWidget::spiritChosen, this, &GameStageWidget::refreshAllComponents);
    connect(spiritCard, &SpiritCardWidget::spiritCubePlacementRequested, this, &GameStageWidget::onSpiritCubePlacementRequested);
    connect(playerInfos, &PlayerInfosWidget::turnEnded, this, &GameStageWidget::onTurnEnded);

    // Câblage Placement Cube Animal
    connect(ownedCards, &PlayerOwnedCardsWidget::cardClicked, this, &GameStageWidget::onAnimalCardSelected);
    connect(personalBoard, &PersonalBoardWidget::cubePlaced, this, &GameStageWidget::onCubePlaced);

    updateInteractionLock();
}

void GameStageWidget::onAnimalCardSelected(int index) {
    clearSpiritCubeSelection();
    personalBoard->setSelectedAnimalCardIndex(index);
    ownedCards->setSelectedIndex(index);
}

void GameStageWidget::onSpiritCubePlacementRequested() {
    clearAnimalCardSelection();
    personalBoard->setSelectedSpiritCube(true);
    spiritCard->setCubePlacementSelected(true);
}

void GameStageWidget::onCubePlaced() {
    clearAnimalCardSelection();
    clearSpiritCubeSelection();
}

void GameStageWidget::onTurnEnded() {
    clearAnimalCardSelection();
    clearSpiritCubeSelection();
    refreshAllComponents();
}

void GameStageWidget::clearAnimalCardSelection() {
    personalBoard->setSelectedAnimalCardIndex(-1);
    ownedCards->setSelectedIndex(-1);
}

void GameStageWidget::clearSpiritCubeSelection() {
    personalBoard->setSelectedSpiritCube(false);
    spiritCard->setCubePlacementSelected(false);
}

void GameStageWidget::refreshAllComponents() {
    personalBoard->updateUI();
    centralBoard->updateUI();
    cardMarket->updateUI();
    ownedCards->updateUI();
    spiritCard->updateUI();
    playerInfos->updateUI();
    updateInteractionLock();

    if (game->isGameOver() && !endGameShown) {
        clearAnimalCardSelection();
        endGameShown = true;
        showEndGameScreen();
    }
}

void GameStageWidget::showEndGameScreen() {
    EndGameDialog dlg(game, this);
    dlg.exec();

    if (dlg.getChoice() == EndGameDialog::Choice::NewGame) {
        Q_EMIT requestNewGame();
    } else if (dlg.getChoice() == EndGameDialog::Choice::ReturnToMenu) {
        Q_EMIT requestReturnToMenu();
    } else if (dlg.getChoice() == EndGameDialog::Choice::QuitApplication) {
        Q_EMIT requestQuitApplication();
    }
}

void GameStageWidget::updateInteractionLock() {
    if (!game) {
        return;
    }

    const bool gameOver = game->isGameOver();

    personalBoard->setEnabled(!gameOver);
    centralBoard->setEnabled(!gameOver);
    cardMarket->setEnabled(!gameOver);
    ownedCards->setEnabled(!gameOver);
    spiritCard->setEnabled(!gameOver);
    playerInfos->setEnabled(!gameOver);
}
