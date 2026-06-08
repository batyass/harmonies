#ifndef SPIRITCARDWIDGET_H
#define SPIRITCARDWIDGET_H

#include <QWidget>

namespace harmonies { namespace core { class Game; } }

class QVBoxLayout;

class SpiritCardWidget : public QWidget {
    Q_OBJECT
private:
    harmonies::core::Game *game;
    QVBoxLayout *contentLayout;

    void clearContent();
    void rebuildChoiceView();
    void rebuildSelectedView();

Q_SIGNALS:
    void spiritChosen();

public:
    explicit SpiritCardWidget(harmonies::core::Game *backendGame, QWidget *parent = nullptr);
    ~SpiritCardWidget() = default;

    void updateUI();
};

#endif // SPIRITCARDWIDGET_H
