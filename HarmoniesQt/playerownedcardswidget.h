#ifndef PLAYEROWNEDCARDSWIDGET_H
#define PLAYEROWNEDCARDSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>
#include "core/Game.h"

/**
 * @class PlayerOwnedCardsWidget
 * @brief Handles the modular rendering of the active player's acquired animal cards and their cube capacities.
 */
class PlayerOwnedCardsWidget : public QWidget {
    Q_OBJECT
private:
    harmonies::core::Game *game;
    QVBoxLayout *mainLayout;
    QWidget *cardsContainer;
    QHBoxLayout *cardsLayout;

public:
    explicit PlayerOwnedCardsWidget(harmonies::core::Game *backendGame, QWidget *parent = nullptr);
    ~PlayerOwnedCardsWidget() = default;

    // Redraws the player's personal animal cards dynamically when slots change
    void updateUI();
};

#endif // PLAYEROWNEDCARDSWIDGET_H
