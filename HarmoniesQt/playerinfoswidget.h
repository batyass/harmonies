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
    QLabel *gameStateLabel;

    QLabel *pendingTitleLabel;
    QWidget *pendingContainer;
    QHBoxLayout *pendingLayout;

    QPushButton *endTurnButton;

    // TRACKING: Holds the currently selected index within game->getPendingTokens()
    static std::size_t selectedTokenIndex;

    QString getColorStyleByTokenType(int typeInt);

Q_SIGNALS:
    void turnEnded();
    void selectionChanged(); // Notifies the board that active item focus has moved

private Q_SLOTS:
    void onEndTurnClicked();
    void handleTokenSelection(std::size_t index);

public:
    explicit PlayerInfosWidget(harmonies::core::Game *backendGame, QWidget *parent = nullptr);
    ~PlayerInfosWidget() = default;

    // Public static getter so the board can poll exactly which token is active
    static std::size_t getSelectedTokenIndex();
    static void resetSelection();

    void updateUI();
};

#endif // PLAYERINFOSWIDGET_H
