#ifndef SETUPWINDOW_H
#define SETUPWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QSpinBox;
class QComboBox;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QMessageBox;
class QVBoxLayout;
class QFormLayout;
class QHBoxLayout;
QT_END_NAMESPACE

namespace harmonies
{
    namespace ui
    {
        //class GameControllerQt;

        class SetupWindow : public QDialog
        {
            Q_OBJECT

        private:
            QSpinBox *nbPlayerBox;
            QComboBox *boardChoiceBox;
            QLineEdit *playerName1Box;
            QLineEdit *playerName2Box;
            QLineEdit *playerName3Box;
            QLineEdit *playerName4Box;
            QPushButton *buttonNext;


        public:
            explicit SetupWindow(QWidget *parent = 0);
        };
    }
}

#endif // SETUPWINDOW_H
