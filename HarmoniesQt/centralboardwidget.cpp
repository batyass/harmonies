#include "model/CentralBoard.h"
#include "model/TokenSlot.h"
#include "core/Game.h"
#include "centralboardwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QGroupBox>
#include <exception>

CentralBoardWidget::CentralBoardWidget(harmonies::core::Game *backendGame, QWidget *parent)
    : QWidget(parent), game(backendGame)
{
    this->setStyleSheet("background-color: #FFF3E0; border: 1px solid #B0BEC5; border-radius: 4px;");

    // Main structural layout (Strictly Vertical)
    QVBoxLayout *mainVerticalLayout = new QVBoxLayout(this);
    mainVerticalLayout->setSpacing(6);

    QLabel *title = new QLabel("=== PLATEAU CENTRAL ===", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-weight: bold; border: none; font-size: 13px;");
    mainVerticalLayout->addWidget(title);

    // 1. TOP HALF: Active slots layout container
    QWidget *container = new QWidget(this);
    container->setStyleSheet("border: none; background: transparent;");
    slotsLayout = new QVBoxLayout(container);
    slotsLayout->setSpacing(4);
    mainVerticalLayout->addWidget(container);

    mainVerticalLayout->addStretch(); // Push elements up/down nicely

    // 2. BOTTOM HALF: Horizontal Legend Key Panel (No right-side invasion!)
    QGroupBox *legendGroup = new QGroupBox("Legend / Guide", this);
    legendGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; color: #5D4037; border: 1px solid #D7CCC8; "
        "border-radius: 6px; margin-top: 10px; padding-top: 10px; background-color: #FAFAFA; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 3px; }"
        );
    // Horizontal row layout for definitions
    QHBoxLayout *legendHorizontalLayout = new QHBoxLayout(legendGroup);
    legendHorizontalLayout->setContentsMargins(6, 6, 6, 6);
    legendHorizontalLayout->setSpacing(12);

    struct LegendItem { QString color; QString letter; QString desc; bool darkText; };
    std::vector<LegendItem> items = {
        {"#2196F3", "W", "Water", false},
        {"#9E9E9E", "S", "Stone", false},
        {"#795548", "E", "Earth", false},
        {"#4CAF50", "T", "Tree", false},
        {"#FFEB3B", "F", "Field", true},
        {"#E53935", "B", "Build", false}
    };

    for (const auto& item : items) {
        QHBoxLayout *itemLayout = new QHBoxLayout();
        itemLayout->setSpacing(4);

        QLabel *dot = new QLabel(legendGroup);
        dot->setFixedSize(20, 20);
        dot->setAlignment(Qt::AlignCenter);
        dot->setText(item.letter);
        dot->setStyleSheet(QString(
                               "background-color: %1; border-radius: 10px; border: 1px solid #424242; "
                               "font-weight: bold; font-size: 10px; color: %2;"
                               ).arg(item.color).arg(item.darkText ? "black" : "white"));
        itemLayout->addWidget(dot);

        QLabel *descLabel = new QLabel(item.desc, legendGroup);
        descLabel->setStyleSheet("border: none; font-size: 10px; font-weight: normal; color: #4E342E;");
        itemLayout->addWidget(descLabel);

        legendHorizontalLayout->addLayout(itemLayout);
    }

    mainVerticalLayout->addWidget(legendGroup);

    updateUI();
}

void CentralBoardWidget::updateUI() {
    if (!game) return;
    clearLayout(slotsLayout);

    harmonies::model::CentralBoard* cb = game->getCentralBoard();
    if (!cb) return;

    std::size_t totalSlots = cb->getNbSlots();
    for (std::size_t i = 0; i < totalSlots; ++i) {
        const harmonies::model::TokenSlot* slot = cb->getSlot(i);
        if (!slot || slot->isEmpty()) continue;

        QWidget *slotRow = new QWidget(this);
        slotRow->setStyleSheet("border: none; background: transparent;");
        QHBoxLayout *rowLayout = new QHBoxLayout(slotRow);
        rowLayout->setContentsMargins(4, 2, 4, 2);

        const auto& tokens = slot->getTokens();
        for (auto t : tokens) {
            QLabel *circle = new QLabel(slotRow);
            circle->setFixedSize(26, 26);
            circle->setAlignment(Qt::AlignCenter);

            QString color;
            QString text;
            switch(static_cast<int>(t)) {
            case 0: color = "#2196F3"; text = "W"; break;
            case 1: color = "#9E9E9E"; text = "S"; break;
            case 2: color = "#795548"; text = "E"; break;
            case 3: color = "#4CAF50"; text = "T"; break;
            case 4: color = "#FFEB3B"; text = "F"; break;
            case 5: color = "#E53935"; text = "B"; break;
            default: color = "#FFFFFF"; text = "?"; break;
            }

            circle->setText(text);
            circle->setStyleSheet(QString(
                                      "background-color: %1; border-radius: 13px; border: 1px solid #616161; "
                                      "font-weight: bold; font-size: 11px; color: %2;"
                                      ).arg(color).arg(static_cast<int>(t) == 4 ? "black" : "white"));

            rowLayout->addWidget(circle);
        }

        rowLayout->addStretch();

        QPushButton *btn = new QPushButton(QString("Prendre %1").arg(i + 1), slotRow);
        btn->setFixedWidth(80);
        btn->setEnabled(game->getState() == harmonies::core::GameState::WaitingForSlotChoice);
        btn->setStyleSheet(
            "QPushButton { background-color: #FFF; border: 1px solid #B0BEC5; border-radius: 4px; padding: 2px; font-size: 11px; }"
            "QPushButton:enabled:hover { background-color: #FFE0B2; }"
            "QPushButton:disabled { background-color: #E0E0E0; color: #9E9E9E; }"
            );

        connect(btn, &QPushButton::clicked, this, [this, i]() {
            try {
                if (game->takeTokensFromSlot(i)) {
                    Q_EMIT slotSelected();
                } else {
                    QMessageBox::warning(this, "Action Impossible", "Erreur lors de la selection.");
                }
            } catch (const std::exception &e) {
                QMessageBox::critical(this, "Erreur du moteur", QString::fromUtf8(e.what()));
            } catch (...) {
                QMessageBox::critical(this, "Erreur du moteur", "Une erreur inattendue est survenue lors de la selection du slot.");
            }
        });
        rowLayout->addWidget(btn);
        slotsLayout->addWidget(slotRow);
    }
}

void CentralBoardWidget::clearLayout(QLayout *layout) {
    if (!layout) return;
    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *widget = item->widget()) widget->deleteLater();
        if (QLayout *child = item->layout()) clearLayout(child);
        delete item;
    }
}
