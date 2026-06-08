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
#include <exception>
#include <limits>

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
    QPoint boardOffset = buildBoardOffset(cells, radius);

    for (const auto& pair : cells) {
        const harmonies::utils::HexCoord& coord = pair.first;
        const harmonies::model::BoardCell& cell = pair.second;

        QPoint pixelPos = boardOffset + axialToPixel(coord.getQ(), coord.getR(), radius);

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

        if (cell.hasCube()) {
            QRect cubeRect(pixelPos.x() - 8, pixelPos.y() - 8, 16, 16);
            painter.setPen(QPen(QColor("#5D4037"), 1));
            painter.setBrush(QColor("#FFB74D"));
            painter.drawRoundedRect(cubeRect, 3, 3);

            // Small highlight to keep the cube visible on both dark and light stacks.
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(255, 255, 255, 140));
            painter.drawEllipse(pixelPos.x() - 3, pixelPos.y() - 3, 5, 5);
        }
    }
}

void PersonalBoardWidget::mousePressEvent(QMouseEvent *event) {
    if (!game || !game->getCurrentPlayer() || !game->getCurrentPlayer()->getBoard()) return;

    int radius = 22;

    const auto& cells = game->getCurrentPlayer()->getBoard()->getCells();
    QPoint boardOffset = buildBoardOffset(cells, radius);

    for (const auto& pair : cells) {
        const harmonies::utils::HexCoord& coord = pair.first;
        QPoint pixelPos = boardOffset + axialToPixel(coord.getQ(), coord.getR(), radius);

        int dx = event->pos().x() - pixelPos.x();
        int dy = event->pos().y() - pixelPos.y();

        if ((dx * dx + dy * dy) < (radius * radius)) {
            auto state = game->getState();
            bool canPlaceAnimalCube =
                state == harmonies::core::GameState::WaitingForSlotChoice ||
                state == harmonies::core::GameState::WaitingForPlacement ||
                state == harmonies::core::GameState::WaitingForTurnEndChoice;

            // If an animal card is selected, the next click on the board targets cube placement.
            if (selectedAnimalCardIndex != -1) {
                if (!canPlaceAnimalCube) {
                    return;
                }

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

            // Otherwise, a board click is interpreted as normal token placement.
            if (state != harmonies::core::GameState::WaitingForPlacement) return;

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

QPoint PersonalBoardWidget::axialToPixel(int q, int r, int radius) const {
    const double verticalStep = radius * 2.1;
    const double horizontalStep = verticalStep * qSqrt(3.0) / 2.0;

    double x = q * horizontalStep;
    // The whole project uses even-q offset coordinates:
    // odd columns are drawn half a row higher than even columns.
    const bool isOddColumn = (q % 2 != 0);
    double y = r * verticalStep - (isOddColumn ? verticalStep / 2.0 : 0.0);
    return QPoint(static_cast<int>(x), static_cast<int>(y));
}

QPoint PersonalBoardWidget::buildBoardOffset(
    const std::map<harmonies::utils::HexCoord, harmonies::model::BoardCell> &cells,
    int radius) const
{
    if (cells.empty()) {
        return QPoint(width() / 2, height() / 2);
    }

    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();

    for (const auto &pair : cells) {
        QPoint rawPos = axialToPixel(pair.first.getQ(), pair.first.getR(), radius);
        minX = std::min(minX, rawPos.x() - radius);
        maxX = std::max(maxX, rawPos.x() + radius);
        minY = std::min(minY, rawPos.y() - radius);
        maxY = std::max(maxY, rawPos.y() + radius);
    }

    const int boardWidth = maxX - minX;
    const int boardHeight = maxY - minY;
    const int topReserved = 48;
    const int bottomMargin = 12;
    const int availableHeight = std::max(0, height() - topReserved - bottomMargin);

    const int offsetX = (width() - boardWidth) / 2 - minX;
    const int offsetY = topReserved + (availableHeight - boardHeight) / 2 - minY;

    return QPoint(offsetX, offsetY);
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
