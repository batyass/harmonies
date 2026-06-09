#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <vector>
#include "core/Game.h"
#include "model/BoardSide.h"

// Forward declaration instead of direct inclusion to break the loop
class GameStageWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    std::unique_ptr<harmonies::core::Game> game;
    GameStageWidget *stageWidget;
    std::vector<std::string> lastPlayerNames;
    harmonies::model::BoardSide lastBoardSide;
    bool lastNatureSpiritEnabled;
    bool hasLastSetup;

    bool createGameFromConfig(const std::vector<std::string> &playerNames,
                              harmonies::model::BoardSide boardSide,
                              bool natureSpiritEnabled);
    bool chooseStartingPlayer(std::vector<std::string> &playerNames);
    bool runSetupAndCreateGame();
    bool restartWithLastSetup();
    void showGameStage();

private Q_SLOTS:
    void onRequestNewGame();
    void onRequestReturnToMenu();
    void onRequestQuitApplication();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

    bool isReady() const;
};

#endif // MAINWINDOW_H
