#include "playerinfoswidget.h"
#include <QMessageBox>
#include <exception>

PlayerInfosWidget::PlayerInfosWidget(harmonies::core::Game *backendGame, QWidget *parent)
    : QWidget(parent), game(backendGame), selectedTokenIndex(0)
{
    this->setStyleSheet("background-color: #E3F2FD; border: 1px solid #B0BEC5; border-radius: 4px;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("=== INFORMATIONS JOUEUR ===", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-weight: bold; border: none;");
    layout->addWidget(title);

    currentPlayerLabel = new QLabel(this);
    currentPlayerLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #1565C0; border: none;");
    layout->addWidget(currentPlayerLabel);

    gameStateLabel = new QLabel(this);
    gameStateLabel->setStyleSheet("font-size: 12px; color: #37474F; border: none;");
    layout->addWidget(gameStateLabel);

    pendingTitleLabel = new QLabel("Jetons a placer (Cliquez pour choisir) :", this);
    pendingTitleLabel->setStyleSheet("font-weight: bold; font-size: 11px; color: #E65100; border: none; margin-top: 5px;");
    layout->addWidget(pendingTitleLabel);

    pendingContainer = new QWidget(this);
    pendingContainer->setStyleSheet("border: none; background: transparent;");
    pendingLayout = new QHBoxLayout(pendingContainer);
    pendingLayout->setContentsMargins(0, 2, 0, 2);
    pendingLayout->setSpacing(8);
    pendingLayout->addStretch();
    layout->addWidget(pendingContainer);

    layout->addStretch();

    endTurnButton = new QPushButton("Finir le Tour", this);
    endTurnButton->setStyleSheet("background-color: #2196F3; color: white; font-weight: bold; padding: 6px; border-radius: 4px;");
    connect(endTurnButton, &QPushButton::clicked, this, &PlayerInfosWidget::onEndTurnClicked);
    layout->addWidget(endTurnButton);

    // ============================================================================
    // MINIMAL BLOCK ADDITION: Append cards tray securely below end-turn section
    // ============================================================================
    QLabel *ownedTitleLabel = new QLabel("Mes Cartes Animales :", this);
    ownedTitleLabel->setStyleSheet("font-weight: bold; font-size: 11px; color: #37474F; margin-top: 6px; border: none;");
    layout->addWidget(ownedTitleLabel);

    ownedCardsContainer = new QWidget(this);
    ownedCardsContainer->setStyleSheet("background-color: #FAFAFA; border: 1px dashed #CFD8DC; border-radius: 4px;");
    ownedCardsLayout = new QHBoxLayout(ownedCardsContainer);
    ownedCardsLayout->setContentsMargins(4, 4, 4, 4);
    ownedCardsLayout->setSpacing(6);
    ownedCardsLayout->addStretch();
    layout->addWidget(ownedCardsContainer);

    updateUI();
}

std::size_t PlayerInfosWidget::getSelectedTokenIndex() const {
    return selectedTokenIndex;
}

void PlayerInfosWidget::resetSelection() {
    selectedTokenIndex = 0;
}

void PlayerInfosWidget::handleTokenSelection(std::size_t index) {
    selectedTokenIndex = index;
    updateUI(); // Redraw borders instantly to show selection highlight
    Q_EMIT selectionChanged();
}

void PlayerInfosWidget::updateUI() {
    if (!game) return;

    harmonies::model::Player* current = game->getCurrentPlayer();
    if (current) {
        currentPlayerLabel->setText(QString("Tour de : %1").arg(QString::fromStdString(current->getName())));
    }

    QString stateStr;
    bool enableEndTurn = false;
    switch(game->getState()) {
    case harmonies::core::GameState::NotStarted: stateStr = "Statut : Non lance"; break;
    case harmonies::core::GameState::WaitingForSlotChoice: stateStr = "Action : Choisir un slot au centre"; break;
    case harmonies::core::GameState::WaitingForPlacement: stateStr = "Action : Placer les jetons sur le plateau"; break;
    case harmonies::core::GameState::WaitingForTurnEndChoice:
        stateStr = "Action : Actions optionnelles ou fin";
        enableEndTurn = true;
        break;
    case harmonies::core::GameState::GameOver: stateStr = "Statut : Partie terminee !"; break;
    }
    gameStateLabel->setText(stateStr);
    endTurnButton->setEnabled(enableEndTurn);

    if(enableEndTurn) {
        endTurnButton->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 6px;");
    } else {
        endTurnButton->setStyleSheet("background-color: #B0BEC5; color: #757575; padding: 6px;");
    }

    // Dynamic generation of pending token selection options
    while (QLayoutItem *item = pendingLayout->takeAt(0)) {
        if (QWidget *w = item->widget()) w->deleteLater();
        delete item;
    }
    pendingLayout->addStretch();

    const std::vector<harmonies::model::TokenType>& pendingList = game->getPendingTokens();

    if (pendingList.empty()) {
        QLabel *noneLabel = new QLabel("Aucun (Prendre un slot)", pendingContainer);
        noneLabel->setStyleSheet("color: #90A4AE; font-style: italic; border: none; font-size: 11px;");
        pendingLayout->insertWidget(0, noneLabel);
        resetSelection();
    } else {
        // Fix safety boundary constraints if index falls out of bounds during placements
        if (selectedTokenIndex >= pendingList.size()) {
            selectedTokenIndex = 0;
        }

        for (std::size_t i = 0; i < pendingList.size(); ++i) {
            ClickableTokenLabel *circle = new ClickableTokenLabel(i, pendingContainer);
            circle->setFixedSize(26, 26);
            circle->setAlignment(Qt::AlignCenter);

            int typeInt = static_cast<int>(pendingList[i]);
            QString color = getColorStyleByTokenType(typeInt);

            QString text;
            switch(typeInt) {
            case 0: text = "W"; break;
            case 1: text = "S"; break;
            case 2: text = "E"; break;
            case 3: text = "T"; break;
            case 4: text = "F"; break;
            case 5: text = "B"; break;
            }
            circle->setText(text);

            // GORGEOUS SELECTION BORDER: If selected, paint thick golden/orange border, otherwise normal thin border
            QString borderStyle = (i == selectedTokenIndex)
                                      ? "border: 3px solid #FF5722; font-size: 11px;"
                                      : "border: 1px solid #37474F; font-size: 10px;";

            circle->setStyleSheet(QString(
                                      "background-color: %1; border-radius: 13px; font-weight: bold; color: %2; %3"
                                      ).arg(color).arg(typeInt == 4 ? "black" : "white").arg(borderStyle));

            connect(circle, &ClickableTokenLabel::tokenClicked, this, &PlayerInfosWidget::handleTokenSelection);

            pendingLayout->insertWidget(pendingLayout->count() - 1, circle);
        }
    }

    // MINIMAL INTEGRATION: Safely invoke the card layout redraw mechanism
    updateOwnedCardsUI();
}

QString PlayerInfosWidget::getColorStyleByTokenType(int typeInt) {
    switch(typeInt) {
    case 0: return "#2196F3";
    case 1: return "#9E9E9E";
    case 2: return "#795548";
    case 3: return "#4CAF50";
    case 4: return "#FFEB3B";
    case 5: return "#E53935";
    default: return "#FFFFFF";
    }
}

void PlayerInfosWidget::onEndTurnClicked() {
    if (!game) return;
    try {
        if (game->endTurn()) {
            resetSelection();
            Q_EMIT turnEnded();
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de finir le tour maintenant.");
        }
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Erreur du moteur", QString::fromUtf8(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Erreur du moteur", "Une erreur inattendue est survenue lors de la fin du tour.");
    }
}

// ============================================================================
// REDRAW FUNCTION: Renders live player card vector states safely
// ============================================================================
void PlayerInfosWidget::updateOwnedCardsUI() {
    if (!game) return;

    while (QLayoutItem *item = ownedCardsLayout->takeAt(0)) {
        if (QWidget *w = item->widget()) w->deleteLater();
        delete item;
    }
    ownedCardsLayout->addStretch();

    harmonies::model::Player* currentPlayer = game->getCurrentPlayer();
    if (!currentPlayer || !currentPlayer->getAnimalCards()) return;

    // Fetch matching data stream directly from model collection
    const std::vector<harmonies::model::AnimalCard>& ownedCards = currentPlayer->getAnimalCards()->getCards();

    if (ownedCards.empty()) {
        QLabel *emptyLabel = new QLabel("Aucune carte acquise", ownedCardsContainer);
        emptyLabel->setStyleSheet("color: #90A4AE; font-style: italic; font-size: 11px; border: none; background: transparent;");
        ownedCardsLayout->insertWidget(0, emptyLabel);
    } else {
        for (std::size_t i = 0; i < ownedCards.size(); ++i) {
            QFrame *cardFrame = new QFrame(ownedCardsContainer);
            cardFrame->setFixedSize(95, 55);
            cardFrame->setStyleSheet(
                "background-color: #E8F5E9; border: 2px solid #81C784; "
                "border-radius: 4px; padding: 2px;"
                );

            QVBoxLayout *frameLayout = new QVBoxLayout(cardFrame);
            frameLayout->setContentsMargins(2, 2, 2, 2);
            frameLayout->setSpacing(1);

            QString cardName = QString::fromStdString(ownedCards[i].getName());
            QLabel *nameLabel = new QLabel(cardName, cardFrame);
            nameLabel->setStyleSheet("font-weight: bold; font-size: 10px; color: #1B5E20; border: none; background: transparent;");
            nameLabel->setAlignment(Qt::AlignCenter);
            frameLayout->addWidget(nameLabel);

            // Dynamic progression checking: tokens placed over max allocation ceiling
            int currentCubes = ownedCards[i].getCubesOnCard();
            int totalCubes = ownedCards[i].totalSlots();

            QLabel *cubeLabel = new QLabel(QString("Cubes: %1/%2").arg(currentCubes).arg(totalCubes), cardFrame);
            cubeLabel->setStyleSheet("font-size: 9px; color: #2E7D32; font-weight: bold; border: none; background: transparent;");
            cubeLabel->setAlignment(Qt::AlignCenter);
            frameLayout->addWidget(cubeLabel);

            ownedCardsLayout->insertWidget(ownedCardsLayout->count() - 1, cardFrame);
        }
    }
}
