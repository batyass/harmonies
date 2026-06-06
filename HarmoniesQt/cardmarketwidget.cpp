#include "cardmarketwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

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

    for (std::size_t i = 0; i < visibleCards.size(); ++i) {
        ClickableCardLabel *cardLabel = new ClickableCardLabel(i, this);
        cardLabel->setFixedSize(90, 130);
        cardLabel->setAlignment(Qt::AlignCenter);

        // Fetch real descriptive parameters from backend structures
        QString animalName = QString::fromStdString(visibleCards[i].getName());

        const std::vector<int>& scores = visibleCards[i].getSlotValues();
        QString scoreStr;
        for (int s : scores) {
            if (!scoreStr.isEmpty()) scoreStr += "/";
            scoreStr += QString::number(s);
        }

        cardLabel->setText(QString("%1\n\n[Pts: %2]").arg(animalName).arg(scoreStr));
        cardLabel->setStyleSheet(
            "background-color: #FFF59D; border: 2px solid #FBC02D; "
            "border-radius: 6px; font-weight: bold; color: #5D4037; font-size: 11px;"
            );

        connect(cardLabel, &ClickableCardLabel::cardClicked, this, &CardMarketWidget::onMarketCardClicked);

        // Insert directly inside the bounded layout
        cardsLayout->insertWidget(cardsLayout->count() - 1, cardLabel);
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

    auto state = game->getState();
    if (state != harmonies::core::GameState::WaitingForSlotChoice &&
        state != harmonies::core::GameState::WaitingForPlacement &&
        state != harmonies::core::GameState::WaitingForTurnEndChoice)
    {
        QMessageBox::information(this, "Action Impossible", "Vous ne pouvez pas prendre de carte durant cette phase.");
        return;
    }

    if (game->takeVisibleAnimalCard(index)) {
        QMessageBox::information(this, "Succes", "Vous avez recupere cette carte animale !");
        Q_EMIT marketUpdated();
        updateUI();
    } else {
        QMessageBox::warning(this, "Erreur", "Vous avez deja pris une carte animale durant ce tour.");
    }
}
