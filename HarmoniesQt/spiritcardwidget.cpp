#include "spiritcardwidget.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

#include "core/Game.h"
#include "model/Player.h"
#include "model/NatureSpiritCard.h"
#include "scoring/NatureSpiritEffect.h"

namespace
{
    QString spiritCardImagePath(const std::string &cardName)
    {
        return QString(":/assets/cards/spirit/%1.png").arg(QString::fromStdString(cardName));
    }

    QString tokenTypeName(harmonies::model::TokenType type)
    {
        switch (type)
        {
        case harmonies::model::TokenType::BlueWater:
            return "Eau";
        case harmonies::model::TokenType::GrayStone:
            return "Montagne";
        case harmonies::model::TokenType::BrownEarth:
            return "Terre";
        case harmonies::model::TokenType::GreenTree:
            return "Arbre";
        case harmonies::model::TokenType::YellowField:
            return "Champ";
        case harmonies::model::TokenType::RedBuilding:
            return "Batiment";
        }

        return "Paysage";
    }

    QString buildEffectSummary(const harmonies::model::NatureSpiritCard &card)
    {
        const harmonies::scoring::NatureSpiritEffect &effect = card.getEffect();

        if (!effect.connectedRules.empty()) {
            QStringList parts;
            for (std::size_t i = 0; i < effect.connectedRules.size(); ++i) {
                const harmonies::scoring::ConnectedGroupRule &rule = effect.connectedRules[i];
                QString sizeText;
                if (rule.minGroupSize == rule.maxGroupSize && rule.maxGroupSize != 0) {
                    sizeText = QString::number(rule.minGroupSize);
                } else if (rule.maxGroupSize == 0) {
                    sizeText = QString("%1+").arg(rule.minGroupSize);
                } else {
                    sizeText = QString("%1-%2").arg(rule.minGroupSize).arg(rule.maxGroupSize);
                }

                parts << QString("%1 : %2 pts")
                             .arg(sizeText)
                             .arg(rule.score);
            }

            return QString("%1 connecte : %2")
                .arg(tokenTypeName(effect.connectedRules[0].type))
                .arg(parts.join(" | "));
        }

        if (!effect.landscapeRules.empty()) {
            QStringList parts;
            for (std::size_t i = 0; i < effect.landscapeRules.size(); ++i) {
                const harmonies::scoring::LandscapeCountRule &rule = effect.landscapeRules[i];
                const QString heightText = (rule.requiredHeight == 0)
                                               ? "toute hauteur"
                                               : QString("hauteur %1").arg(rule.requiredHeight);
                parts << QString("%1 : %2 pts")
                             .arg(heightText)
                             .arg(rule.score);
            }

            return QString("%1 : %2")
                .arg(tokenTypeName(effect.landscapeRules[0].type))
                .arg(parts.join(" | "));
        }

        return "Effet a verifier";
    }
}

SpiritCardWidget::SpiritCardWidget(harmonies::core::Game *backendGame, QWidget *parent)
    : QWidget(parent), game(backendGame), contentLayout(nullptr)
{
    setStyleSheet("background-color: #F8F5E9; border: 1px solid #C9B37E; border-radius: 4px;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);

    QLabel *title = new QLabel("=== CARTE ESPRIT ===", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-weight: bold; border: none; color: #6D4C1F; font-size: 11px;");
    mainLayout->addWidget(title);

    QWidget *contentArea = new QWidget(this);
    contentArea->setStyleSheet("border: none; background: transparent;");
    contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setContentsMargins(0, 4, 0, 0);
    contentLayout->setSpacing(6);
    mainLayout->addWidget(contentArea);

    updateUI();
}

void SpiritCardWidget::setCubePlacementSelected(bool selected)
{
    cubePlacementSelected = selected;
    updateUI();
}

void SpiritCardWidget::clearContent()
{
    while (QLayoutItem *item = contentLayout->takeAt(0)) {
        if (QWidget *w = item->widget()) {
            w->deleteLater();
        } else if (QLayout *l = item->layout()) {
            while (QLayoutItem *sub = l->takeAt(0)) {
                if (QWidget *w = sub->widget()) w->deleteLater();
                delete sub;
            }
        }
        delete item;
    }
}

void SpiritCardWidget::updateUI()
{
    if (!game || !game->isNatureSpiritEnabled()) {
        hide();
        return;
    }

    clearContent();

    harmonies::model::Player *currentPlayer = game->getCurrentPlayer();
    if (currentPlayer == nullptr) {
        hide();
        return;
    }

    const std::vector<harmonies::model::NatureSpiritCard> &spirits = currentPlayer->getNatureSpiritCards();
    if (spirits.empty()) {
        QLabel *emptyLabel = new QLabel("Aucun Esprit", this);
        emptyLabel->setStyleSheet("color: #7E57C2; font-style: italic; font-size: 10px; border: none;");
        emptyLabel->setAlignment(Qt::AlignCenter);
        contentLayout->addWidget(emptyLabel);
        show();
        return;
    }

    show();

    if (spirits.size() > 1) {
        rebuildChoiceView();
    } else {
        rebuildSelectedView();
    }
}

void SpiritCardWidget::rebuildChoiceView()
{
    harmonies::model::Player *currentPlayer = game->getCurrentPlayer();
    const std::vector<harmonies::model::NatureSpiritCard> &spirits = currentPlayer->getNatureSpiritCards();

    QLabel *hintLabel = new QLabel("Choisissez votre carte Esprit pour la partie.", this);
    hintLabel->setWordWrap(true);
    hintLabel->setAlignment(Qt::AlignCenter);
    hintLabel->setStyleSheet(
        "background-color: #FFF8E1; color: #6D4C41; border: 1px solid #E6C97A; "
        "border-radius: 6px; padding: 6px; font-size: 10px; font-weight: bold;");
    contentLayout->addWidget(hintLabel);

    QHBoxLayout *row = new QHBoxLayout();
    row->setSpacing(8);

    for (std::size_t i = 0; i < spirits.size(); ++i) {
        QFrame *cardFrame = new QFrame(this);
        cardFrame->setStyleSheet("background-color: #FFFDF6; border: 2px solid #D7C089; border-radius: 6px;");
        cardFrame->setFixedSize(128, 222);

        QVBoxLayout *frameLayout = new QVBoxLayout(cardFrame);
        frameLayout->setContentsMargins(5, 5, 5, 5);
        frameLayout->setSpacing(4);

        QLabel *imageLabel = new QLabel(cardFrame);
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setFixedSize(112, 158);
        QPixmap pixmap(spiritCardImagePath(spirits[i].getName()));
        if (!pixmap.isNull()) {
            imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageLabel->setStyleSheet("border: none; background: transparent;");
        } else {
            imageLabel->setText(QString::fromStdString(spirits[i].getName()));
            imageLabel->setWordWrap(true);
            imageLabel->setStyleSheet("border: 1px dashed #B0BEC5; background: #FFFDE7; color: #6D4C41;");
        }
        frameLayout->addWidget(imageLabel, 0, Qt::AlignHCenter);

        QLabel *effectLabel = new QLabel(buildEffectSummary(spirits[i]), cardFrame);
        effectLabel->setWordWrap(true);
        effectLabel->setAlignment(Qt::AlignCenter);
        effectLabel->setStyleSheet("font-size: 8px; color: #5D4037; border: none; font-weight: bold;");
        frameLayout->addWidget(effectLabel);

        QPushButton *chooseButton = new QPushButton("Choisir", cardFrame);
        chooseButton->setStyleSheet(
            "QPushButton { background-color: #D7B55C; color: white; font-weight: bold; border-radius: 4px; padding: 4px; font-size: 10px; }"
            "QPushButton:hover { background-color: #C79E34; }");
        connect(chooseButton, &QPushButton::clicked, this, [this, i]() {
            try {
                if (game->chooseNatureSpiritCard(i)) {
                    Q_EMIT spiritChosen();
                    updateUI();
                } else {
                    QMessageBox::warning(this, "Action Impossible", "Vous ne pouvez plus choisir d'Esprit.");
                }
            } catch (const std::exception &e) {
                QMessageBox::critical(this, "Erreur du moteur", QString::fromUtf8(e.what()));
            } catch (...) {
                QMessageBox::critical(this, "Erreur du moteur", "Une erreur inattendue est survenue lors du choix de l'esprit.");
            }
        });
        frameLayout->addWidget(chooseButton);

        row->addWidget(cardFrame);
    }

    contentLayout->addLayout(row);
}

void SpiritCardWidget::rebuildSelectedView()
{
    harmonies::model::Player *currentPlayer = game->getCurrentPlayer();
    const std::vector<harmonies::model::NatureSpiritCard> &spirits = currentPlayer->getNatureSpiritCards();
    const harmonies::model::NatureSpiritCard &card = spirits[0];

    QFrame *cardFrame = new QFrame(this);
    cardFrame->setStyleSheet(QString(
        "background-color: %1; border: %2; border-radius: 6px;")
                                 .arg(cubePlacementSelected ? "#FFF8E1" : "#FFFDF6")
                                 .arg(cubePlacementSelected ? "3px solid #C79E34" : "2px solid #D7C089"));

    QVBoxLayout *frameLayout = new QVBoxLayout(cardFrame);
    frameLayout->setContentsMargins(6, 6, 6, 6);
    frameLayout->setSpacing(5);

    QLabel *imageLabel = new QLabel(cardFrame);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setFixedSize(118, 168);
    QPixmap pixmap(spiritCardImagePath(card.getName()));
    if (!pixmap.isNull()) {
        imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imageLabel->setStyleSheet("border: none; background: transparent;");
    } else {
        imageLabel->setText(QString::fromStdString(card.getName()));
        imageLabel->setWordWrap(true);
        imageLabel->setStyleSheet("border: 1px dashed #B0BEC5; background: #FFFDE7; color: #6D4C41;");
    }
    frameLayout->addWidget(imageLabel, 0, Qt::AlignHCenter);

    QLabel *statusLabel = new QLabel(card.isCubePlaced() ? "Cube deja pose sur le plateau" : "Cube encore present sur la carte", cardFrame);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setWordWrap(true);
    statusLabel->setStyleSheet(
        QString("font-size: 9px; font-weight: bold; border: none; color: %1;")
            .arg(card.isCubePlaced() ? "#2E7D32" : "#8D6E63"));
    frameLayout->addWidget(statusLabel);

    QLabel *effectLabel = new QLabel(buildEffectSummary(card), cardFrame);
    effectLabel->setWordWrap(true);
    effectLabel->setAlignment(Qt::AlignCenter);
    effectLabel->setStyleSheet("font-size: 8px; color: #5D4037; border: none;");
    frameLayout->addWidget(effectLabel);

    if (!card.isCubePlaced()) {
        QPushButton *selectButton = new QPushButton(
            cubePlacementSelected ? "Cube esprit selectionne" : "Selectionner le cube esprit",
            cardFrame);
        selectButton->setStyleSheet(
            QString("QPushButton { background-color: %1; color: white; font-weight: bold; border-radius: 4px; padding: 4px; font-size: 10px; }"
                    "QPushButton:hover { background-color: %2; }")
                .arg(cubePlacementSelected ? "#8D6E63" : "#D7B55C")
                .arg(cubePlacementSelected ? "#795548" : "#C79E34"));
        connect(selectButton, &QPushButton::clicked, this, [this]() {
            cubePlacementSelected = true;
            Q_EMIT spiritCubePlacementRequested();
            updateUI();
        });
        frameLayout->addWidget(selectButton);
    }

    contentLayout->addWidget(cardFrame);
}
