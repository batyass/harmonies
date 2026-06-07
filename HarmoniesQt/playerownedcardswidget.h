#ifndef PLAYEROWNEDCARDSWIDGET_H
#define PLAYEROWNEDCARDSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>
#include <QFrame>
#include <QMouseEvent>
#include "core/Game.h"

/**
 * @class ClickableCardFrame
 * @brief Simple clickable frame for animal cards.
 */
class ClickableCardFrame : public QFrame {
    Q_OBJECT
private:
    int cardIndex;
    bool isClickable;
public:
    explicit ClickableCardFrame(int index, bool clickable, QWidget *parent = nullptr)
        : QFrame(parent), cardIndex(index), isClickable(clickable) {}
Q_SIGNALS:
    void clicked(int index);
protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (!isClickable) return;
        Q_UNUSED(event);
        Q_EMIT clicked(cardIndex);
    }
};

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
    int selectedIndex = -1;

Q_SIGNALS:
    void cardClicked(int index);

public:
    explicit PlayerOwnedCardsWidget(harmonies::core::Game *backendGame, QWidget *parent = nullptr);
    ~PlayerOwnedCardsWidget() = default;

    void setSelectedIndex(int index) { selectedIndex = index; updateUI(); }

    // Redraws the player's personal animal cards dynamically when slots change
    void updateUI();
};

#endif // PLAYEROWNEDCARDSWIDGET_H
