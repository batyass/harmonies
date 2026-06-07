#include "gamestagewidget.h"
#include "personalboardwidget.h"
#include "centralboardwidget.h"
#include "cardmarketwidget.h"
#include "playerownedcardswidget.h"
#include "playerinfoswidget.h"
#include "core/Game.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

GameStageWidget::GameStageWidget(harmonies::core::Game *backendGame, QWidget *parent)
    : QWidget(parent), game(backendGame)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    personalBoard = new PersonalBoardWidget(game, this);
    mainLayout->addWidget(personalBoard, 4);

    centralBoard = new CentralBoardWidget(game, this);
    mainLayout->addWidget(centralBoard, 4);

    QVBoxLayout *rightColumn = new QVBoxLayout();

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
    connect(playerInfos, &PlayerInfosWidget::turnEnded, this, &GameStageWidget::refreshAllComponents);
}

void GameStageWidget::refreshAllComponents() {
    personalBoard->updateUI();
    centralBoard->updateUI();
    cardMarket->updateUI();
    ownedCards->updateUI();
    playerInfos->updateUI();
}
