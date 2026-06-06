#include <QApplication>

#include "ui/qt/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    harmonies::ui::MainWindow window;
    window.show();

    return app.exec();
}
