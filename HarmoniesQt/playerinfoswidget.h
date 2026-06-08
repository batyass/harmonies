#ifndef PLAYERINFOSWIDGET_H
#define PLAYERINFOSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "core/Game.h"

// Custom clickable label class to represent a selective pending token
class ClickableTokenLabel : public QLabel {
    Q_OBJECT
private:
    std::size_t tokenIndex;
public:
    explicit ClickableTokenLabel(std::size_t index, QWidget *parent = nullptr)
        : QLabel(parent), tokenIndex(index) {}
    ~ClickableTokenLabel() = default;

Q_SIGNALS:
    void tokenClicked(std::size_t index);

protected:
    void mousePressEvent(QMouseEvent *event) override {
        Q_UNUSED(event);
        Q_EMIT tokenClicked(tokenIndex);
    }
};

class PlayerInfosWidget : public QWidget {
    Q_OBJECT
private:
    harmonies::core::Game *game;
    QLabel *currentPlayerLabel;
    QLabel *turnLabel;
    QLabel *gameStateLabel;
    QLabel *phaseHintLabel;
    QLabel *finalRoundLabel;

    QLabel *pendingTitleLabel;
    QWidget *pendingContainer;
    QHBoxLayout *pendingLayout;

    QPushButton *endTurnButton;

    // TRACKING: Holds the currently selected index within game->getPendingTokens()
    std::size_t selectedTokenIndex;

    QString getColorStyleByTokenType(int typeInt);
    QString buildStateText() const;
    QString buildPhaseHintText() const;
    QString buildPendingTitleText() const;
    int getCurrentPlayerPosition() const;

Q_SIGNALS:
    void turnEnded();
    void selectionChanged(); // Notifies the board that active item focus has moved

private Q_SLOTS:
    void onEndTurnClicked();
    void handleTokenSelection(std::size_t index);

public:
    explicit PlayerInfosWidget(harmonies::core::Game *backendGame, QWidget *parent = nullptr);
    ~PlayerInfosWidget() = default;

    std::size_t getSelectedTokenIndex() const;
    void resetSelection();

    void updateUI();
};

#endif // PLAYERINFOSWIDGET_H
