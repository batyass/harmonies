#include "endgamedialog.h"
#include "core/Game.h"
#include "model/Player.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <algorithm>
#include <vector>

EndGameDialog::EndGameDialog(harmonies::core::Game *game, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Fin de Partie");
    setMinimumWidth(340);

    const auto &players = game->getPlayers();

    struct Entry {
        const harmonies::model::Player *player;
        int score;
    };
    std::vector<Entry> entries;
    entries.reserve(players.size());
    for (const auto &p : players)
        entries.push_back({p.get(), static_cast<int>(game->calculatePlayerScore(*p))});

    std::sort(entries.begin(), entries.end(),
              [](const Entry &a, const Entry &b) { return a.score > b.score; });

    const Entry *winner = entries.empty() ? nullptr : &entries[0];

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(8);

    QLabel *title = new QLabel("=== FIN DE PARTIE ===", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 20px; font-weight: bold; margin-bottom: 8px;");
    layout->addWidget(title);

    if (winner) {
        QLabel *winnerLabel = new QLabel(
            QString("Vainqueur : %1  (%2 pts)")
                .arg(QString::fromStdString(winner->player->getName()))
                .arg(winner->score),
            this);
        winnerLabel->setAlignment(Qt::AlignCenter);
        winnerLabel->setStyleSheet(
            "font-size: 15px; font-weight: bold; color: #E65100; margin-bottom: 4px;");
        layout->addWidget(winnerLabel);
    }

    QFrame *sep = new QFrame(this);
    sep->setFrameShape(QFrame::HLine);
    sep->setFrameShadow(QFrame::Sunken);
    layout->addWidget(sep);

    for (std::size_t i = 0; i < entries.size(); ++i) {
        bool isWinner = (winner && entries[i].player == winner->player);
        QLabel *row = new QLabel(
            QString("%1.  %2  —  %3 pts")
                .arg(i + 1)
                .arg(QString::fromStdString(entries[i].player->getName()))
                .arg(entries[i].score),
            this);
        row->setAlignment(Qt::AlignCenter);
        row->setStyleSheet(isWinner
            ? "font-size: 14px; font-weight: bold; color: #2E7D32; padding: 2px;"
            : "font-size: 13px; color: #37474F; padding: 2px;");
        layout->addWidget(row);
    }

    layout->addStretch();

    QPushButton *closeBtn = new QPushButton("Fermer", this);
    closeBtn->setStyleSheet(
        "background-color: #E53935; color: white; font-weight: bold; padding: 8px; border-radius: 4px;");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeBtn);
}
