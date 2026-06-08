#ifndef PERSONALBOARDWIDGET_H
#define PERSONALBOARDWIDGET_H

#include <QWidget>
#include <map>

#include "model/BoardCell.h"
#include "utils/hexCoord.h"

namespace harmonies { namespace core { class Game; } }
class PlayerInfosWidget;

class PersonalBoardWidget : public QWidget {
    Q_OBJECT
private:
    harmonies::core::Game *game;
    PlayerInfosWidget *playerInfosWidget;
    int selectedAnimalCardIndex = -1;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPoint axialToPixel(int q, int r, int radius) const;
    QPoint buildBoardOffset(const std::map<harmonies::utils::HexCoord, harmonies::model::BoardCell> &cells,
                            int radius) const;
    QString getColorByTokenType(int typeInt);

Q_SIGNALS:
    void boardUpdated();
    void cubePlaced(); // Signal to clear selection in UI

public:
    explicit PersonalBoardWidget(harmonies::core::Game *backendGame, QWidget *parent = nullptr);
    ~PersonalBoardWidget() = default;

    void setPlayerInfosWidget(PlayerInfosWidget *infosWidget);
    void setSelectedAnimalCardIndex(int index) { selectedAnimalCardIndex = index; }
    void updateUI();
};

#endif // PERSONALBOARDWIDGET_H
