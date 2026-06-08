#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "core/Game.h"

// Forward declaration instead of direct inclusion to break the loop
class GameStageWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    std::unique_ptr<harmonies::core::Game> game;
    GameStageWidget *stageWidget;

    bool runSetupAndCreateGame();
    void showGameStage();

private Q_SLOTS:
    void onRequestNewGame();
    void onRequestReturnToMenu();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

    bool isReady() const;
};

#endif // MAINWINDOW_H
