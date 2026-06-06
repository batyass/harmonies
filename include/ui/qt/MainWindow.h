#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
QT_END_NAMESPACE

namespace harmonies
{
    namespace ui
    {
        class GameControllerQt;

        class MainWindow : public QMainWindow
        {
            Q_OBJECT

        private:
            GameControllerQt *controller;
            QLabel *titleLabel;
            QLabel *statusLabel;
            QPushButton *newGameButton;

            void refreshUi();

        public:
            explicit MainWindow(QWidget *parent = 0);
        };
    }
}

#endif
