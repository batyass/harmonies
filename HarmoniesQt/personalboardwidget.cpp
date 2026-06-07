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
#include <algorithm>
#include <climits>
#include <exception>

PersonalBoardWidget::PersonalBoardWidget(harmonies::core::Game *backendGame, QWidget *parent)
    : QWidget(parent), game(backendGame), playerInfosWidget(nullptr)
{
    this->setStyleSheet("background-color: #E8F5E9; border: 1px solid #B0BEC5; border-radius: 4px;");
}

void PersonalBoardWidget::setPlayerInfosWidget(PlayerInfosWidget *infosWidget) {
    playerInfosWidget = infosWidget;
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

    int qMin = INT_MAX;
    for (const auto& pair : cells)
        qMin = std::min(qMin, pair.first.getQ());

    for (const auto& pair : cells) {
        const harmonies::utils::HexCoord& coord = pair.first;
        const harmonies::model::BoardCell& cell = pair.second;

        QPoint pixelPos = axialToPixel(coord.getQ(), coord.getR(), radius, centerX, centerY, qMin);

        // Aligned with BoardCell.h methods: getTokenStack()
        if (cell.getTokenStack().empty()) {
            painter.setPen(QPen(QColor("#78909C"), 1, Qt::DashLine));
            painter.setBrush(QColor("#F5F5F5"));
            painter.drawEllipse(pixelPos, radius - 2, radius - 2);

            painter.setPen(QColor("#90A4AE"));
            painter.setFont(QFont("Arial", 7));
            painter.drawText(QRect(pixelPos.x()-15, pixelPos.y()-10, 30, 20), Qt::AlignCenter, QString("%1,%2").arg(coord.getQ()).arg(coord.getR()));
        } else {
            const auto& stack = cell.getTokenStack();
            const int outerR = radius - 1;
            static const double scales[3] = {1.0, 0.62, 0.30};
            for (int i = 0; i < static_cast<int>(stack.size()); ++i) {
                int r = static_cast<int>(outerR * scales[i]);
                painter.setBrush(QColor(getColorByTokenType(static_cast<int>(stack[i]))));
                painter.setPen(QPen(Qt::black, 1));
                painter.drawEllipse(pixelPos, r, r);
            }
        }
    }
}

void PersonalBoardWidget::mousePressEvent(QMouseEvent *event) {
    if (!game) return;

    int radius = 22;
    int centerX = width() / 2;
    int centerY = height() / 2 + 20;

    const auto& cells = game->getCurrentPlayer()->getBoard()->getCells();

    int qMin = INT_MAX;
    for (const auto& pair : cells)
        qMin = std::min(qMin, pair.first.getQ());

    for (const auto& pair : cells) {
        const harmonies::utils::HexCoord& coord = pair.first;
        QPoint pixelPos = axialToPixel(coord.getQ(), coord.getR(), radius, centerX, centerY, qMin);

        int dx = event->pos().x() - pixelPos.x();
        int dy = event->pos().y() - pixelPos.y();

        if ((dx * dx + dy * dy) < (radius * radius)) {
            // STEP 3: Handle Animal Cube Placement if a card is selected
            if (selectedAnimalCardIndex != -1) {
                try {
                    if (game->placeAnimalCube(static_cast<std::size_t>(selectedAnimalCardIndex), coord)) {
                        selectedAnimalCardIndex = -1;
                        Q_EMIT cubePlaced();
                        Q_EMIT boardUpdated();
                        return;
                    } else {
                        QMessageBox::warning(this, "Erreur", "Placement de cube non valide pour cette carte.");
                        // Reset selection on error too to allow user to try again or change strategy
                        selectedAnimalCardIndex = -1;
                        Q_EMIT cubePlaced();
                        return;
                    }
                } catch (const std::exception &e) {
                    QMessageBox::critical(this, "Erreur du moteur", QString::fromUtf8(e.what()));
                    selectedAnimalCardIndex = -1;
                    Q_EMIT cubePlaced();
                    return;
                }
            }

            // FALLBACK: Normal Token Placement logic
            if (game->getState() != harmonies::core::GameState::WaitingForPlacement) return;

            const auto& pending = game->getPendingTokens();
            if (!pending.empty()) {
                // CLEAN EXTRACTION: Get index selected by player from the list
                std::size_t targetIndex = 0;
                if (playerInfosWidget != nullptr) {
                    targetIndex = playerInfosWidget->getSelectedTokenIndex();
                }

                // Safety bounds sanitization fallback
                if (targetIndex >= pending.size()) {
                    targetIndex = 0;
                }

                harmonies::model::TokenType tokenToPlace = pending[targetIndex];

                try {
                    if (game->placeTokenOnBoard(coord, tokenToPlace)) {
                        // Reset focus smoothly to remaining elements to prevent index overflow crashes
                        if (playerInfosWidget != nullptr) {
                            playerInfosWidget->resetSelection();
                        }
                        Q_EMIT boardUpdated();
                        return;
                    } else {
                        QMessageBox::warning(this, "Erreur", "Placement non valide selon les regles.");
                    }
                } catch (const std::exception &e) {
                    QMessageBox::critical(this, "Erreur du moteur", QString::fromUtf8(e.what()));
                } catch (...) {
                    QMessageBox::critical(this, "Erreur du moteur", "Une erreur inattendue est survenue lors du placement du jeton.");
                }
            }
            break;
        }
    }
}

QPoint PersonalBoardWidget::axialToPixel(int q, int r, int radius, int centerX, int centerY, int qMin) {
    double colWidth  = radius * 2.1;
    double rowHeight = radius * 2.1;

    double x = centerX + q * colWidth;
    // Use column index (q - qMin) to determine stagger so the pattern is always
    // "even index = straight, odd index = shifted up", regardless of board side.
    bool isOddColumn = ((q - qMin) % 2 != 0);
    double y = centerY + r * rowHeight - (isOddColumn ? rowHeight / 2.0 : 0.0);
    return QPoint(static_cast<int>(x), static_cast<int>(y));
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
