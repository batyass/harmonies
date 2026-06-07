#include "setupdialog.h"
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QComboBox>

SetupDialog::SetupDialog(QWidget *parent) : QDialog(parent) {
    this->setWindowTitle("Configuration de la Partie");
    this->resize(400, 350);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 1. Player Count Selection
    QHBoxLayout *countLayout = new QHBoxLayout();
    countLayout->addWidget(new QLabel("Nombre de joueurs :"));
    playerCountSpinBox = new QSpinBox(this);
    playerCountSpinBox->setRange(1, 4); // Harmonies supports up to 4 players
    playerCountSpinBox->setValue(2);    // Default to 2 players
    countLayout->addWidget(playerCountSpinBox);
    mainLayout->addLayout(countLayout);

    // 2. Container for Dynamic Player Name Inputs
    QLabel *namesLabel = new QLabel("Noms des joueurs :", this);
    mainLayout->addWidget(namesLabel);

    QWidget *namesWidget = new QWidget(this);
    namesContainerLayout = new QVBoxLayout(namesWidget);
    mainLayout->addWidget(namesWidget);

    // 3. Board Side Selection
    QHBoxLayout *sideLayout = new QHBoxLayout();
    sideLayout->addWidget(new QLabel("Face du plateau :"));
    boardSideComboBox = new QComboBox(this);
    boardSideComboBox->addItem("Face A", QVariant('A'));
    boardSideComboBox->addItem("Face B", QVariant('B'));
    sideLayout->addWidget(boardSideComboBox);
    mainLayout->addLayout(sideLayout);

    // 4. Game Options (Nature Spirit)
    natureSpiritCheckBox = new QCheckBox("Activer l'extension Esprits de la Nature", this);
    natureSpiritCheckBox->setChecked(false);
    mainLayout->addWidget(natureSpiritCheckBox);

    // 4. Action Button
    QPushButton *startButton = new QPushButton("Lancer la Partie", this);
    mainLayout->addWidget(startButton);

    // Connect signals and slots
    connect(playerCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SetupDialog::onPlayerCountChanged);
    connect(startButton, &QPushButton::clicked, this, &SetupDialog::onStartGame);

    // Initialize with default player count fields
    onPlayerCountChanged(playerCountSpinBox->value());
}

void SetupDialog::onPlayerCountChanged(int count) {
    // Clear previous inputs and remove their layout items to avoid stale spacing.
    while (QLayoutItem *item = namesContainerLayout->takeAt(0)) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }

    nameInputs.clear();

    // Dynamically generate QLineEdit rows based on current spinbox value
    for (int i = 0; i < count; ++i) {
        QLineEdit *input = new QLineEdit(this);
        input->setPlaceholderText(QString("Joueur %1").arg(i + 1));
        namesContainerLayout->addWidget(input);
        nameInputs.push_back(input);
    }
}

void SetupDialog::onStartGame() {
    // Basic verification: ensure no names are completely blank
    for (int i = 0; i < nameInputs.size(); ++i) {
        if (nameInputs[i]->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Erreur", QString("Veuillez entrer un nom pour le Joueur %1.").arg(i + 1));
            return;
        }
    }
    this->accept(); // Close dialog with positive result
}

std::vector<std::string> SetupDialog::getPlayerNames() const {
    std::vector<std::string> names;
    for (QLineEdit *input : nameInputs) {
        names.push_back(input->text().trimmed().toStdString());
    }
    return names;
}

harmonies::model::BoardSide SetupDialog::getBoardSide() const {
    return (boardSideComboBox->currentData().toChar() == 'A')
        ? harmonies::model::BoardSide::A
        : harmonies::model::BoardSide::B;
}

bool SetupDialog::isNatureSpiritEnabled() const {
    return natureSpiritCheckBox->isChecked();
}
