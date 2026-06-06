#ifndef PERSONALBOARDWIDGET_H
#define PERSONALBOARDWIDGET_H

#include <QWidget>

namespace harmonies { namespace core { class Game; } }

class PersonalBoardWidget : public QWidget {
    Q_OBJECT
private:
    harmonies::core::Game *game;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPoint axialToPixel(int q, int r, int radius, int centerX, int centerY, int qMin);
    QString getColorByTokenType(int typeInt);

Q_SIGNALS:
    void boardUpdated();

public:
    explicit PersonalBoardWidget(harmonies::core::Game *backendGame, QWidget *parent = nullptr);
    ~PersonalBoardWidget() = default;

    void updateUI();
};

#endif // PERSONALBOARDWIDGET_H
