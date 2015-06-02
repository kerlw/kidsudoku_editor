#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //center the main window on desktop, maybe fail because of Qt's layze calculating.
    QRect rect = w.rect();
    rect.moveCenter(QApplication::desktop()->availableGeometry().center());
    w.move(rect.x(), rect.y());

    w.show();

    return a.exec();
}
