#include "personalboardwidget.h"
#include "core/Game.h"
#include "model/PersonalBoard.h"
#include "model/BoardCell.h"
#include "utils/hexCoord.h"
#include "playerinfoswidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#include <QMessageBox>

PersonalBoardWidget::PersonalBoardWidget(harmonies::core::Game *backendGame, QWidget *parent)
    : QWidget(parent), game(backendGame)
{
    this->setStyleSheet("background-color: #E8F5E9; border: 1px solid #B0BEC5; border-radius: 4px;");
}

void PersonalBoardWidget::updateUI() {
    this->update();
}

void PersonalBoardWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!game || !game->getCurrentPlayer() || !game->getCurrentPlayer()->getBoard()) return;

    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(rect(), Qt::AlignTop | Qt::AlignHCenter, "\n=== PLATEAU PERSONNEL ===");

    const harmonies::model::PersonalBoard* board = game->getCurrentPlayer()->getBoard();
    const auto& cells = board->getCells();

    int radius = 22;
    int centerX = width() / 2;
    int centerY = height() / 2 + 20;

    for (const auto& pair : cells) {
        const harmonies::utils::HexCoord& coord = pair.first;
        const harmonies::model::BoardCell& cell = pair.second;

        QPoint pixelPos = axialToPixel(coord.getQ(), coord.getR(), radius, centerX, centerY);

        // Aligned with BoardCell.h methods: getTokenStack()
        if (cell.getTokenStack().empty()) {
            painter.setPen(QPen(QColor("#78909C"), 1, Qt::DashLine));
            painter.setBrush(QColor("#F5F5F5"));
            painter.drawEllipse(pixelPos, radius - 2, radius - 2);

            painter.setPen(QColor("#90A4AE"));
            painter.setFont(QFont("Arial", 7));
            painter.drawText(QRect(pixelPos.x()-15, pixelPos.y()-10, 30, 20), Qt::AlignCenter, QString("%1,%2").arg(coord.getQ()).arg(coord.getR()));
        } else {
            painter.setPen(QPen(Qt::black, 1));
            // Top token is the last element of the stack vector
            int topType = static_cast<int>(cell.getTokenStack().back());
            painter.setBrush(QColor(getColorByTokenType(topType)));
            painter.drawEllipse(pixelPos, radius - 1, radius - 1);
        }
    }
}

void PersonalBoardWidget::mousePressEvent(QMouseEvent *event) {
    if (!game || game->getState() != harmonies::core::GameState::WaitingForPlacement) return;

    int radius = 22;
    int centerX = width() / 2;
    int centerY = height() / 2 + 20;

    const auto& cells = game->getCurrentPlayer()->getBoard()->getCells();

    for (const auto& pair : cells) {
        const harmonies::utils::HexCoord& coord = pair.first;
        QPoint pixelPos = axialToPixel(coord.getQ(), coord.getR(), radius, centerX, centerY);

        int dx = event->pos().x() - pixelPos.x();
        int dy = event->pos().y() - pixelPos.y();

        // Perimeter hit-test inside clicked hexagon radius limits
        if ((dx * dx + dy * dy) < (radius * radius)) {
            const auto& pending = game->getPendingTokens();
            if (!pending.empty()) {
                // CLEAN EXTRACTION: Get index selected by player from the list
                std::size_t targetIndex = PlayerInfosWidget::getSelectedTokenIndex();

                // Safety bounds sanitization fallback
                if (targetIndex >= pending.size()) {
                    targetIndex = 0;
                }

                harmonies::model::TokenType tokenToPlace = pending[targetIndex];

                if (game->placeTokenOnBoard(coord, tokenToPlace)) {
                    // Reset focus smoothly to remaining elements to prevent index overflow crashes
                    PlayerInfosWidget::resetSelection();
                    Q_EMIT boardUpdated();
                    return;
                } else {
                    QMessageBox::warning(this, "Erreur", "Placement non valide selon les regles.");
                }
            }
            break;
        }
    }
}

QPoint PersonalBoardWidget::axialToPixel(int q, int r, int radius, int centerX, int centerY) {
    double size = radius * 1.15;
    double x = size * (qSqrt(3.0) * q + qSqrt(3.0)/2.0 * r);
    double y = size * (3.0/2.0 * r);
    return QPoint(static_cast<int>(x) + centerX, static_cast<int>(y) + centerY);
}

QString PersonalBoardWidget::getColorByTokenType(int typeInt) {
    switch(typeInt) {
    case 0: return "#2196F3"; // BlueWater
    case 1: return "#9E9E9E"; // GrayStone
    case 2: return "#795548"; // BrownEarth
    case 3: return "#4CAF50"; // GreenTree
    case 4: return "#FFEB3B"; // YellowField
    case 5: return "#E53935"; // RedBuilding
    default: return "#FFFFFF";
    }
}
