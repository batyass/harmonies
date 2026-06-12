#include "endgamedialog.h"
#include "core/Game.h"
#include "model/Player.h"
#include "model/ScoreReport.h"
#include "scoring/LandscapeScoreCalculator.h"
#include "scoring/AnimalCardScoreCalculator.h"
#include "scoring/NatureSpiritScoreCalculator.h"
#include "scoring/SoloScoreEvaluator.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <algorithm>
#include <vector>

static harmonies::model::ScoreReport computeReport(const harmonies::model::Player &p)
{
    harmonies::model::ScoreReport report = harmonies::scoring::LandscapeScoreReport(*p.getBoard());

    const harmonies::model::PlayerCardCollection *animalCards = p.getAnimalCards();
    if (animalCards) {
        for (const auto &card : animalCards->getCards())
            report.addAnimalsScore(harmonies::scoring::AnimalCardScoreCalculator(card));
    }

    for (const auto &card : p.getNatureSpiritCards())
        report.addSpiritScore(harmonies::scoring::NatureSpiritScoreCalculator(card, *p.getBoard()));

    return report;
}

static QLabel *makeRow(const QString &label, std::size_t pts)
{
    QLabel *l = new QLabel(QString("  %1 : %2 pts").arg(label).arg(pts));
    l->setStyleSheet("font-size: 12px; color: #546E7A; padding: 1px 0;");
    return l;
}

struct Entry {
    const harmonies::model::Player *player;
    harmonies::model::ScoreReport report;
};

static void addPlayerCard(QVBoxLayout *col, const Entry &e, std::size_t rank, bool isWinner)
{
    QFrame *sep = new QFrame();
    sep->setFrameShape(QFrame::HLine);
    sep->setFrameShadow(QFrame::Sunken);
    col->addWidget(sep);

    const harmonies::model::ScoreReport &r = e.report;

    QLabel *header = new QLabel(
        QString("%1.  %2  —  %3 pts")
            .arg(rank)
            .arg(QString::fromStdString(e.player->getName()))
            .arg(r.getTotalScore()));
    header->setStyleSheet(isWinner
        ? "font-size: 14px; font-weight: bold; color: #2E7D32;"
        : "font-size: 14px; font-weight: bold; color: #37474F;");
    col->addWidget(header);

    col->addWidget(makeRow("Arbres",    r.getTreeScore()));
    col->addWidget(makeRow("Montagnes", r.getMountainScore()));
    col->addWidget(makeRow("Champs",    r.getFieldScore()));
    col->addWidget(makeRow("Batiments", r.getBuildingScore()));
    col->addWidget(makeRow("Eau",       r.getWaterScore()));
    col->addWidget(makeRow("Animaux",   r.getAnimalsScore()));
    if (r.getSpiritScore() > 0)
        col->addWidget(makeRow("Esprit", r.getSpiritScore()));
}

EndGameDialog::EndGameDialog(harmonies::core::Game *game, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Fin de Partie");

    const auto &players = game->getPlayers();

    std::vector<Entry> entries;
    entries.reserve(players.size());
    for (const auto &p : players)
        entries.push_back({p.get(), computeReport(*p)});

    std::sort(entries.begin(), entries.end(),
              [](const Entry &a, const Entry &b) {
                  return a.report.getTotalScore() > b.report.getTotalScore();
              });

    const Entry *winner = entries.empty() ? nullptr : &entries[0];

    QVBoxLayout *root = new QVBoxLayout(this);
    root->setSpacing(8);

    QLabel *title = new QLabel("=== FIN DE PARTIE ===", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 20px; font-weight: bold; margin-bottom: 4px;");
    root->addWidget(title);

    if (winner) {
        QLabel *winnerLabel = new QLabel(
            QString("Vainqueur : %1  (%2 pts)")
                .arg(QString::fromStdString(winner->player->getName()))
                .arg(winner->report.getTotalScore()));
        winnerLabel->setAlignment(Qt::AlignCenter);
        winnerLabel->setStyleSheet(
            "font-size: 15px; font-weight: bold; color: #E65100; margin-bottom: 2px;");
        root->addWidget(winnerLabel);
    }

    if (players.size() == 1 && !entries.empty()) {
        const harmonies::model::Player *soloPlayer = entries[0].player;
        const std::size_t suns = harmonies::scoring::evaluateSoloScore(
            entries[0].report.getTotalScore(),
            soloPlayer->getBoard()->getSide(),
            game->isNatureSpiritEnabled());

        QLabel *soloLabel = new QLabel(
            QString("Resultat solo : %1 soleil(s)")
                .arg(suns),
            this);
        soloLabel->setAlignment(Qt::AlignCenter);
        soloLabel->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #C68A00; "
            "background-color: #FFF8E1; border: 1px solid #F0D27A; border-radius: 6px; padding: 6px;");
        root->addWidget(soloLabel);
    }

    QHBoxLayout *columns = new QHBoxLayout();
    columns->setSpacing(16);

    QVBoxLayout *leftCol  = new QVBoxLayout();
    QVBoxLayout *rightCol = new QVBoxLayout();
    leftCol->setSpacing(4);
    rightCol->setSpacing(4);

    for (std::size_t i = 0; i < entries.size(); ++i) {
        bool isWinner = (winner && entries[i].player == winner->player);
        QVBoxLayout *target = (i < 2) ? leftCol : rightCol;
        addPlayerCard(target, entries[i], i + 1, isWinner);
    }

    leftCol->addStretch();
    rightCol->addStretch();

    columns->addLayout(leftCol, 1);
    if (entries.size() > 2) {
        QFrame *vSep = new QFrame(this);
        vSep->setFrameShape(QFrame::VLine);
        vSep->setFrameShadow(QFrame::Sunken);
        columns->addWidget(vSep);
        columns->addLayout(rightCol, 1);
    }

    root->addLayout(columns);

    QHBoxLayout *buttonRow = new QHBoxLayout();
    buttonRow->setSpacing(10);

    QPushButton *newGameBtn = new QPushButton("Rejouer", this);
    newGameBtn->setStyleSheet(
        "background-color: #43A047; color: white; font-weight: bold; padding: 8px; border-radius: 4px;");
    connect(newGameBtn, &QPushButton::clicked, this, [this]() {
        choice = Choice::NewGame;
        accept();
    });
    buttonRow->addWidget(newGameBtn);

    QPushButton *menuBtn = new QPushButton("Retour a la configuration", this);
    menuBtn->setStyleSheet(
        "background-color: #1E88E5; color: white; font-weight: bold; padding: 8px; border-radius: 4px;");
    connect(menuBtn, &QPushButton::clicked, this, [this]() {
        choice = Choice::ReturnToMenu;
        accept();
    });
    buttonRow->addWidget(menuBtn);

    QPushButton *closeBtn = new QPushButton("Rester sur le plateau", this);
    closeBtn->setStyleSheet(
        "background-color: #757575; color: white; font-weight: bold; padding: 8px; border-radius: 4px;");
    connect(closeBtn, &QPushButton::clicked, this, [this]() {
        choice = Choice::StayOnBoard;
        accept();
    });
    buttonRow->addWidget(closeBtn);

    QPushButton *quitBtn = new QPushButton("Quitter", this);
    quitBtn->setStyleSheet(
        "background-color: #C62828; color: white; font-weight: bold; padding: 8px; border-radius: 4px;");
    connect(quitBtn, &QPushButton::clicked, this, [this]() {
        choice = Choice::QuitApplication;
        accept();
    });
    buttonRow->addWidget(quitBtn);

    root->addLayout(buttonRow);
}
