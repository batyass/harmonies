#include "ui/qt/SetupWindow.h"

#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>

namespace harmonies
{
    namespace ui
    {
        SetupWindow::SetupWindow(QWidget *parent)
            : QDialog(parent)
        {
            QSpinBox *nbPlayerBox = new QSpinBox(this);
            nbPlayerBox->setMinimum(1);
            nbPlayerBox->setMaximum(4);
            QComboBox *boardChoiceBox= new QComboBox(this);
            boardChoiceBox->addItem("A");
            boardChoiceBox->addItem("B");
            QLineEdit *playerName1Box=new QLineEdit(this);
            QLineEdit *playerName2Box=new QLineEdit(this);
            QLineEdit *playerName3Box=new QLineEdit(this);
            QLineEdit *playerName4Box=new QLineEdit(this);
            QPushButton *buttonNext=new QPushButton("continuer", this);
            QFormLayout *formLayout = new QFormLayout;
            formLayout->addRow("Nombre de joueurs (1-4) :", nbPlayerBox);
            formLayout->addRow("Face du plateau :", boardChoiceBox);
            formLayout->addRow("Nom du joueur 1 : :", playerName1Box);
            formLayout->addRow("Nom du joueur 2 : :", playerName2Box);
            formLayout->addRow("Nom du joueur 3 : :", playerName3Box);
            formLayout->addRow("Nom du joueur 4 : :", playerName4Box);

            QVBoxLayout *mainLayout = new QVBoxLayout;
            mainLayout->addLayout(formLayout);
            mainLayout->addWidget(buttonNext);

            setLayout(mainLayout);
            setWindowTitle("Menu de configuration");
            resize(640, 240);
        }
    }
}
