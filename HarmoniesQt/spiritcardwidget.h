#ifndef SPIRITCARDWIDGET_H
#define SPIRITCARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include "core/Game.h"

class SpiritCardWidget : public QWidget {
    Q_OBJECT
private:
    harmonies::core::Game *game;
    QVBoxLayout *contentLayout;

Q_SIGNALS:
    void spiritChosen();

public:
    explicit SpiritCardWidget(harmonies::core::Game *backendGame, QWidget *parent = nullptr)
        : QWidget(parent), game(backendGame) 
    {
        this->setStyleSheet("background-color: #F3E5F5; border: 1px solid #9575CD; border-radius: 4px;");
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(8, 8, 8, 8);

        QLabel *title = new QLabel("=== CARTE ESPRIT ===", this);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet("font-weight: bold; border: none; color: #4527A0; font-size: 11px;");
        mainLayout->addWidget(title);

        QWidget *contentArea = new QWidget(this);
        contentArea->setStyleSheet("border: none; background: transparent;");
        contentLayout = new QVBoxLayout(contentArea);
        contentLayout->setContentsMargins(0, 4, 0, 0);
        contentLayout->setSpacing(4);
        mainLayout->addWidget(contentArea);

        updateUI();
    }

    void updateUI() {
        if (!game || !game->isNatureSpiritEnabled()) {
            this->hide();
            return;
        }

        // Clear layout
        while (QLayoutItem *item = contentLayout->takeAt(0)) {
            if (QWidget *w = item->widget()) w->deleteLater();
            delete item;
        }

        harmonies::model::Player* currentPlayer = game->getCurrentPlayer();
        if (!currentPlayer) return;

        const std::vector<harmonies::model::NatureSpiritCard>& spirits = currentPlayer->getNatureSpiritCards();
        
        if (spirits.empty()) {
            QLabel *emptyLabel = new QLabel("Aucun Esprit", this);
            emptyLabel->setStyleSheet("color: #7E57C2; font-style: italic; font-size: 10px; border: none;");
            emptyLabel->setAlignment(Qt::AlignCenter);
            contentLayout->addWidget(emptyLabel);
            this->show();
        } else {
            this->show();
            for (std::size_t i = 0; i < spirits.size(); ++i) {
                if (spirits.size() > 1) {
                    // Choice phase
                    QPushButton *btn = new QPushButton(QString::fromStdString(spirits[i].getName()), this);
                    btn->setStyleSheet(
                        "QPushButton { background-color: #EDE7F6; border: 1px solid #B39DDB; border-radius: 4px; padding: 4px; font-size: 10px; color: #512DA8; }"
                        "QPushButton:hover { background-color: #D1C4E9; }"
                    );
                    connect(btn, &QPushButton::clicked, this, [this, i]() {
                        if (game->chooseNatureSpiritCard(i)) {
                            Q_EMIT spiritChosen();
                            updateUI();
                        } else {
                            QMessageBox::warning(this, "Action Impossible", "Vous ne pouvez plus choisir d'Esprit.");
                        }
                    });
                    contentLayout->addWidget(btn);
                } else {
                    // Display phase
                    QLabel *nameLabel = new QLabel(QString::fromStdString(spirits[i].getName()), this);
                    nameLabel->setStyleSheet("font-weight: bold; font-size: 11px; color: #311B92; border: none;");
                    nameLabel->setAlignment(Qt::AlignCenter);
                    contentLayout->addWidget(nameLabel);
                    
                    QLabel *statusLabel = new QLabel(spirits[i].isCubePlaced() ? "[Cube Pose]" : "[Cube sur Carte]", this);
                    statusLabel->setStyleSheet("font-size: 9px; color: #673AB7; border: none;");
                    statusLabel->setAlignment(Qt::AlignCenter);
                    contentLayout->addWidget(statusLabel);
                }
            }
        }
    }
};

#endif
