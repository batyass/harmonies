#include <QApplication>

//#include "ui/qt/MainWindow.h"
#include "ui/qt/SetupWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //harmonies::ui::MainWindow window;
    //window.show();

    harmonies::ui::SetupWindow setupWindow;
    setupWindow.show();

    return app.exec();
}
