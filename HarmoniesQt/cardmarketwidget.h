#ifndef CARDMARKETWIDGET_H
#define CARDMARKETWIDGET_H

#include <QWidget>

class QHBoxLayout;
namespace harmonies { namespace core { class Game; } }

class CardMarketWidget : public QWidget {
    Q_OBJECT
private:
    harmonies::core::Game *game;
    QHBoxLayout *cardsLayout; // Container to hold drawn card frames

public:
    explicit CardMarketWidget(harmonies::core::Game *backendGame, QWidget *parent = nullptr);
    ~CardMarketWidget() = default;

    // Core function to dynamically fetch backend cards and paint them
    void updateUI();
};

#endif // CARDMARKETWIDGET_H
