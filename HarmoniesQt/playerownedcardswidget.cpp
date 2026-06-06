#include "playerownedcardswidget.h"
#include "model/Player.h"
#include <QFrame>

PlayerOwnedCardsWidget::PlayerOwnedCardsWidget(harmonies::core::Game *backendGame, QWidget *parent)
    : QWidget(parent), game(backendGame)
{
    // Styling the container area for user's personal hand inventory cards
    this->setStyleSheet("background-color: #FAFAFA; border: 1px dashed #CFD8DC; border-radius: 4px;");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(6, 6, 6, 6);

    QLabel *titleLabel = new QLabel("Mes Cartes Animales (En cours) :", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 11px; color: #37474F; border: none;");
    mainLayout->addWidget(titleLabel);

    cardsContainer = new QWidget(this);
    cardsContainer->setStyleSheet("border: none; background: transparent;");
    cardsLayout = new QHBoxLayout(cardsContainer);
    cardsLayout->setContentsMargins(0, 2, 0, 2);
    cardsLayout->setSpacing(8);
    cardsLayout->addStretch();

    mainLayout->addWidget(cardsContainer);
    updateUI();
}

void PlayerOwnedCardsWidget::updateUI() {
    if (!game) return;

    while (QLayoutItem *item = cardsLayout->takeAt(0)) {
        if (QWidget *w = item->widget()) w->deleteLater();
        delete item;
    }
    cardsLayout->addStretch();

    harmonies::model::Player* currentPlayer = game->getCurrentPlayer();
    if (!currentPlayer || !currentPlayer->getAnimalCards()) return;

    // Extracting owned cards from backend PlayerCardCollection asset manager
    const std::vector<harmonies::model::AnimalCard>& ownedCards = currentPlayer->getAnimalCards()->getCards();

    if (ownedCards.empty()) {
        QLabel *emptyLabel = new QLabel("Aucune carte acquise (Action optionnelle)", cardsContainer);
        emptyLabel->setStyleSheet("color: #90A4AE; font-style: italic; font-size: 11px; border: none;");
        cardsLayout->insertWidget(0, emptyLabel);
    } else {
        for (std::size_t i = 0; i < ownedCards.size(); ++i) {
            QFrame *cardFrame = new QFrame(cardsContainer);
            cardFrame->setFixedSize(110, 65);
            cardFrame->setStyleSheet(
                "background-color: #E8F5E9; border: 2px solid #81C784; "
                "border-radius: 4px; padding: 4px;"
                );

            QVBoxLayout *frameLayout = new QVBoxLayout(cardFrame);
            frameLayout->setContentsMargins(2, 2, 2, 2);
            frameLayout->setSpacing(2);

            // Fetch the REAL card name from backend structure
            QString cardName = QString::fromStdString(ownedCards[i].getName());
            QLabel *nameLabel = new QLabel(cardName, cardFrame);
            nameLabel->setStyleSheet("font-weight: bold; font-size: 10px; color: #1B5E20; border: none;");
            nameLabel->setAlignment(Qt::AlignCenter);
            frameLayout->addWidget(nameLabel);

            // Fetch the REAL cube values status tracking securely
            int currentCubes = ownedCards[i].getCubesOnCard();
            int totalCubes = ownedCards[i].totalSlots();

            QLabel *cubeStatusLabel = new QLabel(QString("Cubes: %1 / %2").arg(currentCubes).arg(totalCubes), cardFrame);
            cubeStatusLabel->setStyleSheet("font-size: 10px; color: #2E7D32; font-weight: bold; border: none;");
            cubeStatusLabel->setAlignment(Qt::AlignCenter);
            frameLayout->addWidget(cubeStatusLabel);

            cardsLayout->insertWidget(cardsLayout->count() - 1, cardFrame);
        }
    }
}
