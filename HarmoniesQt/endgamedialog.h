#ifndef ENDGAMEDIALOG_H
#define ENDGAMEDIALOG_H

#include <QDialog>

namespace harmonies { namespace core { class Game; } }

class EndGameDialog : public QDialog {
    Q_OBJECT
public:
    enum class Choice {
        StayOnBoard,
        NewGame,
        ReturnToMenu,
        QuitApplication
    };

private:
    Choice choice = Choice::StayOnBoard;

public:
    explicit EndGameDialog(harmonies::core::Game *game, QWidget *parent = nullptr);

    Choice getChoice() const { return choice; }
};

#endif // ENDGAMEDIALOG_H
