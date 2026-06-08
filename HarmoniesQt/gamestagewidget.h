#ifndef GAMESTAGEWIDGET_H
#define GAMESTAGEWIDGET_H

#include <QWidget>

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

    bool endGameShown = false;

    void clearAnimalCardSelection();
    void showEndGameScreen();
    void updateInteractionLock();

Q_SIGNALS:
    void requestNewGame();
    void requestReturnToMenu();

private Q_SLOTS:
    void onAnimalCardSelected(int index);
    void onCubePlaced();
    void onTurnEnded();

public:
    explicit GameStageWidget(harmonies::core::Game *backendGame, QWidget *parent = nullptr);
    ~GameStageWidget() = default;

    void refreshAllComponents();
};


#endif // GAMESTAGEWIDGET_H
