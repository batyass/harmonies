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

    turnLabel = new QLabel(this);
    turnLabel->setStyleSheet("font-size: 12px; color: #455A64; border: none;");
    layout->addWidget(turnLabel);

    gameStateLabel = new QLabel(this);
    gameStateLabel->setStyleSheet("font-size: 13px; font-weight: bold; color: #37474F; border: none;");
    layout->addWidget(gameStateLabel);

    phaseHintLabel = new QLabel(this);
    phaseHintLabel->setWordWrap(true);
    phaseHintLabel->setStyleSheet("font-size: 11px; color: #546E7A; border: none; background: #F5FAFD; padding: 6px; border-radius: 4px;");
    layout->addWidget(phaseHintLabel);

    finalRoundLabel = new QLabel(this);
    finalRoundLabel->setWordWrap(true);
    finalRoundLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #8A1C1C; background: #FFE8E3; border: 1px solid #F0B7A7; border-radius: 4px; padding: 6px;");
    finalRoundLabel->hide();
    layout->addWidget(finalRoundLabel);

    pendingTitleLabel = new QLabel(this);
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

    if (game->getTurnManager() != nullptr) {
        turnLabel->setText(QString("Tour de table : %1  |  Joueur %2/%3")
                               .arg(game->getTurnManager()->getTurnCount())
                               .arg(getCurrentPlayerPosition())
                               .arg(static_cast<int>(game->getPlayers().size())));
    } else {
        turnLabel->clear();
    }

    gameStateLabel->setText(buildStateText());
    phaseHintLabel->setText(buildPhaseHintText());
    pendingTitleLabel->setText(buildPendingTitleText());

    bool enableEndTurn = false;
    switch(game->getState()) {
    case harmonies::core::GameState::WaitingForTurnEndChoice:
        enableEndTurn = true;
        break;
    default:
        break;
    }
    endTurnButton->setEnabled(enableEndTurn);
    endTurnButton->setText(enableEndTurn ? "Passer au joueur suivant" : "Finir le tour");

    if(enableEndTurn) {
        endTurnButton->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 6px;");
    } else {
        endTurnButton->setStyleSheet("background-color: #B0BEC5; color: #757575; padding: 6px;");
    }

    if (game->isFinalRoundTriggered() && !game->isGameOver()) {
        finalRoundLabel->setText("Dernier tour enclenche : chaque joueur termine encore son tour avant le decompte final.");
        finalRoundLabel->show();
    } else if (game->isGameOver()) {
        finalRoundLabel->setText("La partie est terminee. Consultez le classement final.");
        finalRoundLabel->show();
    } else {
        finalRoundLabel->hide();
    }

    // Rebuild the pending token selection area to match the current turn state.
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
        // Keep the selected index valid while the pending list shrinks during placements.
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
}

QString PlayerInfosWidget::buildStateText() const {
    switch (game->getState()) {
    case harmonies::core::GameState::NotStarted:
        return "Phase : partie non lancee";
    case harmonies::core::GameState::WaitingForSlotChoice:
        return "Phase : choix du groupe de 3 jetons";
    case harmonies::core::GameState::WaitingForPlacement:
        return "Phase : placement obligatoire des jetons";
    case harmonies::core::GameState::WaitingForTurnEndChoice:
        return "Phase : actions optionnelles ou fin du tour";
    case harmonies::core::GameState::GameOver:
        return "Phase : partie terminee";
    }

    return QString();
}

QString PlayerInfosWidget::buildPhaseHintText() const {
    const std::size_t pendingCount = game->getPendingTokens().size();

    switch (game->getState()) {
    case harmonies::core::GameState::NotStarted:
        return "La partie n'a pas encore commence.";
    case harmonies::core::GameState::WaitingForSlotChoice:
        return "Choisissez un slot du plateau central. Les 3 jetons du slot seront pris d'un coup.";
    case harmonies::core::GameState::WaitingForPlacement:
        return QString("Placez vos jetons sur votre plateau personnel. Il en reste %1 a poser.")
            .arg(pendingCount);
    case harmonies::core::GameState::WaitingForTurnEndChoice:
        return "Vous avez fini les actions obligatoires. Vous pouvez encore prendre une carte animale, poser des cubes, ou passer au joueur suivant.";
    case harmonies::core::GameState::GameOver:
        return "Le decompte final est termine.";
    }

    return QString();
}

QString PlayerInfosWidget::buildPendingTitleText() const {
    const std::size_t pendingCount = game->getPendingTokens().size();

    if (pendingCount == 0) {
        return "Jetons en attente : aucun";
    }

    return QString("Jetons en attente : %1 (cliquez pour choisir l'ordre de pose)").arg(pendingCount);
}

int PlayerInfosWidget::getCurrentPlayerPosition() const {
    harmonies::model::Player *current = game->getCurrentPlayer();
    if (current == nullptr) {
        return 0;
    }

    const std::vector<std::unique_ptr<harmonies::model::Player>> &players = game->getPlayers();
    for (std::size_t i = 0; i < players.size(); ++i) {
        if (players[i].get() == current) {
            return static_cast<int>(i + 1);
        }
    }

    return 0;
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
