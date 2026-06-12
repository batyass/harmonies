#ifndef CARDMARKETWIDGET_H
#define CARDMARKETWIDGET_H

#include <QWidget>
#include <QLabel>
#include "core/Game.h"

class QHBoxLayout;

/**
 * @class ClickableCardLabel
 * @brief Standard custom click receiver representing individual market cards.
 */
class ClickableCardLabel : public QLabel {
    Q_OBJECT
private:
    std::size_t slotIndex;
public:
    explicit ClickableCardLabel(std::size_t index, QWidget *parent = nullptr)
        : QLabel(parent), slotIndex(index) {}
    ~ClickableCardLabel() = default;

Q_SIGNALS:
    void cardClicked(std::size_t index);

protected:
    void mousePressEvent(QMouseEvent *event) override {
        Q_UNUSED(event);
        Q_EMIT cardClicked(slotIndex);
    }
};

class CardMarketWidget : public QWidget {
    Q_OBJECT
private:
    harmonies::core::Game *game;
    QHBoxLayout *cardsLayout; // Container to hold drawn card frames

Q_SIGNALS:
    void marketUpdated();

private Q_SLOTS:
    void onMarketCardClicked(std::size_t index);
    void onDiscardCardClicked(std::size_t index);

public:
    explicit CardMarketWidget(harmonies::core::Game *backendGame, QWidget *parent = nullptr);
    ~CardMarketWidget() = default;

    // Core function to dynamically fetch backend cards and paint them
    void updateUI();
};

#endif // CARDMARKETWIDGET_H
