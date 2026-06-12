#include "cardmarketwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <exception>

namespace
{
    QString animalCardImagePath(const std::string &cardName)
    {
        return QString(":/assets/cards/animal/%1.png").arg(QString::fromStdString(cardName));
    }
}

CardMarketWidget::CardMarketWidget(harmonies::core::Game *backendGame, QWidget *parent)
    : QWidget(parent), game(backendGame)
{
    // Maintaining the original background style perfectly
    this->setStyleSheet("background-color: #ECEFF1; border: 1px solid #B0BEC5; border-radius: 4px;");

    // FIX: Main layout handles both the static title and the cards container inside one single box
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);

    QLabel *title = new QLabel("=== CARTES ANIMAL VISIBLES ===", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-weight: bold; border: none; color: #37474F;");
    mainLayout->addWidget(title);

    // DIRECT BINDING: Use a sub-widget to hold the cards row so it strictly stays inside the grey area
    QWidget *cardsRowFrame = new QWidget(this);
    cardsRowFrame->setStyleSheet("border: none; background: transparent;");

    // Wire the framework layout variable defined by teammate
    cardsLayout = new QHBoxLayout(cardsRowFrame);
    cardsLayout->setContentsMargins(0, 0, 0, 0);
    cardsLayout->setSpacing(8);
    cardsLayout->addStretch();

    mainLayout->addWidget(cardsRowFrame, 1); // Allow it to flexibly expand inside the grey container

    updateUI();
}

void CardMarketWidget::updateUI() {
    if (!game || !game->getAnimalCardDeck()) return;

    // Purge historical graphical nodes cleanly
    while (QLayoutItem *item = cardsLayout->takeAt(0)) {
        if (QWidget *w = item->widget()) w->deleteLater();
        delete item;
    }
    cardsLayout->addStretch();

    // Pulling matching data array directly from backend card deck structure
    const std::vector<harmonies::model::AnimalCard>& visibleCards = game->getAnimalCardDeck()->getVisible();

    const bool soloMode = (game->getCentralBoard()->getNbSlots() == 3);
    const bool canReplace = soloMode &&
                            game->getState() == harmonies::core::GameState::WaitingForTurnEndChoice;

    for (std::size_t i = 0; i < visibleCards.size(); ++i) {
        QWidget *cardColumn = new QWidget(this);
        cardColumn->setStyleSheet("border: none; background: transparent;");
        QVBoxLayout *colLayout = new QVBoxLayout(cardColumn);
        colLayout->setContentsMargins(0, 0, 0, 0);
        colLayout->setSpacing(3);

        ClickableCardLabel *cardLabel = new ClickableCardLabel(i, this);
        cardLabel->setFixedSize(100, 170);
        cardLabel->setAlignment(Qt::AlignCenter);

        QString animalName = QString::fromStdString(visibleCards[i].getName());

        const std::vector<int>& scores = visibleCards[i].getSlotValues();
        QString scoreStr;
        for (int s : scores) {
            if (!scoreStr.isEmpty()) scoreStr += "/";
            scoreStr += QString::number(s);
        }

        const QString imagePath = animalCardImagePath(visibleCards[i].getName());
        QPixmap pixmap(imagePath);

        cardLabel->setToolTip(QString("%1\nPoints : %2").arg(animalName).arg(scoreStr));
        if (!pixmap.isNull()) {
            cardLabel->setPixmap(pixmap.scaled(cardLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            cardLabel->setStyleSheet(
                "background-color: white; border: 2px solid #FBC02D; "
                "border-radius: 6px; padding: 2px;"
                );
        } else {
            cardLabel->setText(QString("%1\n\n[Pts: %2]").arg(animalName).arg(scoreStr));
            cardLabel->setStyleSheet(
                "background-color: #FFF59D; border: 2px solid #FBC02D; "
                "border-radius: 6px; font-weight: bold; color: #5D4037; font-size: 11px;"
                );
        }

        connect(cardLabel, &ClickableCardLabel::cardClicked, this, &CardMarketWidget::onMarketCardClicked);
        colLayout->addWidget(cardLabel);

        if (canReplace) {
            QPushButton *discardBtn = new QPushButton("Defausser", cardColumn);
            discardBtn->setStyleSheet(
                "QPushButton { background-color: #EF9A9A; color: #B71C1C; font-weight: bold; "
                "border-radius: 4px; padding: 2px; font-size: 10px; border: 1px solid #E57373; }"
                "QPushButton:hover { background-color: #E57373; }");
            connect(discardBtn, &QPushButton::clicked, this, [this, i]() {
                onDiscardCardClicked(i);
            });
            colLayout->addWidget(discardBtn);
        }

        cardsLayout->insertWidget(cardsLayout->count() - 1, cardColumn);
    }

    if (visibleCards.empty()) {
        QLabel *emptyLabel = new QLabel("Marche de cartes vide", this);
        emptyLabel->setStyleSheet("color: #78909C; font-style: italic; font-size: 11px; border: none;");
        emptyLabel->setAlignment(Qt::AlignCenter);
        cardsLayout->insertWidget(0, emptyLabel);
    }
}

void CardMarketWidget::onMarketCardClicked(std::size_t index) {
    if (!game) return;

    try {
        auto state = game->getState();
        if (state != harmonies::core::GameState::WaitingForSlotChoice &&
            state != harmonies::core::GameState::WaitingForPlacement &&
            state != harmonies::core::GameState::WaitingForTurnEndChoice)
        {
            QMessageBox::information(this, "Action Impossible", "Vous ne pouvez pas prendre de carte durant cette phase.");
            return;
        }

        const harmonies::model::Player *cp = game->getCurrentPlayer();
        const std::size_t activeCards =
            cp->getActiveAnimalCardCount() +
            (cp->hasUnplacedChosenNatureSpiritCard() ? 1u : 0u);
        if (activeCards >= 4) {
            QMessageBox::warning(this, "Limite atteinte",
                "Vous avez deja 4 cartes.");
            return;
        }

        if (game->takeVisibleAnimalCard(index)) {
            QMessageBox::information(this, "Succes", "Vous avez recupere cette carte animale !");
            Q_EMIT marketUpdated();
            updateUI();
        } else {
            QMessageBox::warning(this, "Erreur", "Vous avez deja pris une carte animale durant ce tour.");
        }
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Erreur du moteur", QString::fromUtf8(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Erreur du moteur", "Une erreur inattendue est survenue lors de la prise de la carte animale.");
    }
}

void CardMarketWidget::onDiscardCardClicked(std::size_t index) {
    if (!game) return;
    try {
        if (game->replaceAnimalCard(index)) {
            Q_EMIT marketUpdated();
            updateUI();
        } else {
            QMessageBox::warning(this, "Action Impossible",
                "Vous ne pouvez pas defausser de carte.");
        }
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Erreur du moteur", QString::fromUtf8(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Erreur du moteur", "Une erreur inattendue est survenue.");
    }
}
