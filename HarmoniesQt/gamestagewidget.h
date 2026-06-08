#ifndef GAMESTAGEWIDGET_H
#define GAMESTAGEWIDGET_H

#include <QWidget>
#include <QPushButton>

// Forward declarations
class PersonalBoardWidget;
class CentralBoardWidget;
class CardMarketWidget;
class PlayerOwnedCardsWidget;
class PlayerInfosWidget;
class SpiritCardWidget;

namespace harmonies { namespace core { class Game; } }

class GameStageWidget : public QWidget {
    Q_OBJECT
private:
    harmonies::core::Game *game;

    PersonalBoardWidget *personalBoard;
    CentralBoardWidget *centralBoard;
    CardMarketWidget *cardMarket;
    PlayerOwnedCardsWidget *ownedCards;
    SpiritCardWidget *spiritCard;
    PlayerInfosWidget *playerInfos;
    QPushButton *debugEndButton;

    bool endGameShown = false;

    void clearAnimalCardSelection();
    void showEndGameScreen();

private Q_SLOTS:
    void onAnimalCardSelected(int index);
    void onCubePlaced();
    void onTurnEnded();
    void onDebugEndClicked();

public:
    explicit GameStageWidget(harmonies::core::Game *backendGame, QWidget *parent = nullptr);
    ~GameStageWidget() = default;

    void refreshAllComponents();
};


#endif // GAMESTAGEWIDGET_H
