#include <QtGui/QApplication>
#include "lawnmower.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
    new LAWNMower;
    return a.exec();
}
