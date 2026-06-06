#include "cardmarketwidget.h"
#include <QVBoxLayout>
#include <QLabel>

CardMarketWidget::CardMarketWidget(harmonies::core::Game *backendGame, QWidget *parent)
    : QWidget(parent), game(backendGame)
{
    this->setStyleSheet("background-color: #ECEFF1; border: 1px solid #B0BEC5; border-radius: 4px;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *title = new QLabel("=== CARTES ANIMAL & ESPRIT ===", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-weight: bold; border: none;");
    layout->addWidget(title);

    QLabel *infoText = new QLabel("Cartes visibles configurees\n(Pretes pour l'etape suivante)", this);
    infoText->setAlignment(Qt::AlignCenter);
    infoText->setStyleSheet("color: #78909C; border: none; font-style: italic;");
    layout->addWidget(infoText);

    layout->addStretch();
}

void CardMarketWidget::updateUI() {
    // Placeholder function for next jalon steps
}
