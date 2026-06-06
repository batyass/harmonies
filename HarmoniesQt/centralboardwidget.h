#ifndef CENTRALBOARDWIDGET_H
#define CENTRALBOARDWIDGET_H

#include <QWidget>

class QVBoxLayout;

// Use clean, standard forward declarations instead of including backend headers
namespace harmonies {
namespace core { class Game; }
}

class CentralBoardWidget : public QWidget {
    Q_OBJECT
private:
    harmonies::core::Game *game; // Clean pointer
    QVBoxLayout *slotsLayout;

Q_SIGNALS:
    void slotSelected();

private:
    void clearLayout(QLayout *layout);
    QString getColorStyleByTokenType(int typeInt);

public:
    explicit CentralBoardWidget(harmonies::core::Game *backendGame, QWidget *parent = nullptr);
    ~CentralBoardWidget() = default;

    void updateUI();
};

#endif // CENTRALBOARDWIDGET_H
