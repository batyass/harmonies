#ifndef ENDGAMEDIALOG_H
#define ENDGAMEDIALOG_H

#include <QDialog>

namespace harmonies { namespace core { class Game; } }

class EndGameDialog : public QDialog {
    Q_OBJECT
public:
    explicit EndGameDialog(harmonies::core::Game *game, QWidget *parent = nullptr);
};

#endif // ENDGAMEDIALOG_H
