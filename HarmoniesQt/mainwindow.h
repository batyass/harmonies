#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Forward declaration instead of direct inclusion to break the loop
class GameStageWidget;

namespace harmonies { namespace core { class Game; } }

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    harmonies::core::Game *game;
    GameStageWidget *stageWidget;

public:
    explicit MainWindow(harmonies::core::Game *backendGame, QWidget *parent = nullptr);
    ~MainWindow() = default;
};

#endif // MAINWINDOW_H
