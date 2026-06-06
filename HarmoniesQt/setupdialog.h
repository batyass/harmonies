#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <vector>
#include <string>
#include "model/BoardSide.h"

class SetupDialog : public QDialog {
    Q_OBJECT
private:
    QSpinBox *playerCountSpinBox;
    QVBoxLayout *namesContainerLayout;
    std::vector<QLineEdit*> nameInputs;
    QComboBox *boardSideComboBox;
    QCheckBox *natureSpiritCheckBox;

private Q_SLOTS:
    // Dynamically adjust the number of name input fields when spinbox changes
    void onPlayerCountChanged(int count);
    // Validate inputs and accept the dialog
    void onStartGame();

public:
    explicit SetupDialog(QWidget *parent = nullptr);
    ~SetupDialog() = default;

    // Getters to fetch data perfectly aligned with Game constructor requirements
    std::vector<std::string> getPlayerNames() const;
    harmonies::model::BoardSide getBoardSide() const;
    bool isNatureSpiritEnabled() const;
};

#endif // SETUPDIALOG_H
