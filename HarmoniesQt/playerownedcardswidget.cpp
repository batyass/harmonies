#include "playerownedcardswidget.h"
#include "model/Player.h"
#include <QFrame>
#include <QPixmap>

namespace
{
    QString animalCardImagePath(const std::string &cardName)
    {
        return QString(":/assets/cards/animal/%1.png").arg(QString::fromStdString(cardName));
    }
}

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
        selectedIndex = -1;
    } else {
        for (std::size_t i = 0; i < ownedCards.size(); ++i) {
            bool complete = ownedCards[i].isComplete();
            ClickableCardFrame *cardFrame = new ClickableCardFrame(static_cast<int>(i), !complete, cardsContainer);
            cardFrame->setFixedSize(120, 180);
            
            QString borderStyle;
            QString bgColor;
            
            if (complete) {
                borderStyle = "border: 2px solid #90A4AE;";
                bgColor = "background-color: #ECEFF1;";
            } else if (static_cast<int>(i) == selectedIndex) {
                borderStyle = "border: 2px solid #FF5722;";
                bgColor = "background-color: #C8E6C9;";
            } else {
                borderStyle = "border: 2px solid #81C784;";
                bgColor = "background-color: #E8F5E9;";
            }
            
            cardFrame->setStyleSheet(
                QString("%1 %2 border-radius: 4px; padding: 4px;").arg(borderStyle).arg(bgColor)
                );

            if (!complete) {
                connect(cardFrame, &ClickableCardFrame::clicked, this, &PlayerOwnedCardsWidget::cardClicked);
            }

            QVBoxLayout *frameLayout = new QVBoxLayout(cardFrame);
            frameLayout->setContentsMargins(4, 4, 4, 4);
            frameLayout->setSpacing(4);

            // Fetch the REAL card name from backend structure
            QString cardName = QString::fromStdString(ownedCards[i].getName());
            QLabel *imageLabel = new QLabel(cardFrame);
            imageLabel->setAlignment(Qt::AlignCenter);
            imageLabel->setFixedSize(100, 120);
            imageLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            QPixmap pixmap(animalCardImagePath(ownedCards[i].getName()));
            if (!pixmap.isNull()) {
                imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                imageLabel->setStyleSheet("border: none; background: transparent;");
            } else {
                imageLabel->setText(cardName);
                imageLabel->setWordWrap(true);
                imageLabel->setStyleSheet("font-weight: bold; font-size: 10px; color: #5D4037; border: 1px dashed #B0BEC5; background: #FFFDE7;");
            }
            frameLayout->addWidget(imageLabel);

            QLabel *nameLabel = new QLabel(cardName, cardFrame);
            nameLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            nameLabel->setStyleSheet(QString("font-weight: bold; font-size: 10px; color: %1; border: none;")
                                     .arg(complete ? "#546E7A" : "#1B5E20"));
            nameLabel->setAlignment(Qt::AlignCenter);
            frameLayout->addWidget(nameLabel);

            // Fetch the REAL cube values status tracking securely
            int currentCubes = ownedCards[i].getCubesOnCard();
            int totalCubes = ownedCards[i].totalSlots();

            QLabel *cubeStatusLabel = new QLabel(complete ? "Terminee !" : QString("Cubes: %1 / %2").arg(currentCubes).arg(totalCubes), cardFrame);
            cubeStatusLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            cubeStatusLabel->setStyleSheet(QString("font-size: 10px; color: %1; font-weight: bold; border: none;")
                                           .arg(complete ? "#78909C" : "#2E7D32"));
            cubeStatusLabel->setAlignment(Qt::AlignCenter);
            frameLayout->addWidget(cubeStatusLabel);

            cardsLayout->insertWidget(cardsLayout->count() - 1, cardFrame);
        }
    }
}
