#include "mainwindow.h"
#include <QApplication>
#include <QColor>
#include <QPalette>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("Fusion");

    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor("#F7F7F7"));
    lightPalette.setColor(QPalette::WindowText, Qt::black);
    lightPalette.setColor(QPalette::Base, Qt::white);
    lightPalette.setColor(QPalette::AlternateBase, QColor("#EFEFEF"));
    lightPalette.setColor(QPalette::Text, Qt::black);
    lightPalette.setColor(QPalette::Button, QColor("#EAEAEA"));
    lightPalette.setColor(QPalette::ButtonText, Qt::black);
    lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
    lightPalette.setColor(QPalette::ToolTipText, Qt::black);
    lightPalette.setColor(QPalette::Highlight, QColor("#4A90E2"));
    lightPalette.setColor(QPalette::HighlightedText, Qt::white);
    lightPalette.setColor(QPalette::PlaceholderText, QColor("#757575"));
    a.setPalette(lightPalette);

    MainWindow w;
    if (!w.isReady())
    {
        return 0;
    }

    w.show();
    return a.exec();
}
