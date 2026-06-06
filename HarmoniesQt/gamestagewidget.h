#ifndef GAMESTAGEWIDGET_H
#define GAMESTAGEWIDGET_H

#include <QWidget>

// Forward declarations
class PersonalBoardWidget;
class CentralBoardWidget;
class CardMarketWidget;
class PlayerInfosWidget;

namespace harmonies { namespace core { class Game; } }

class GameStageWidget : public QWidget {
    Q_OBJECT
private:
    harmonies::core::Game *game;

    PersonalBoardWidget *personalBoard;
    CentralBoardWidget *centralBoard;
    CardMarketWidget *cardMarket;
    PlayerInfosWidget *playerInfos;

public:
    explicit GameStageWidget(harmonies::core::Game *backendGame, QWidget *parent = nullptr);
    ~GameStageWidget() = default;

    void refreshAllComponents();
};

#endif // GAMESTAGEWIDGET_H
