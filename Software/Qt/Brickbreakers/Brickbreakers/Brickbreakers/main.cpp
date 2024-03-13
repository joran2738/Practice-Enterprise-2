#include "eventfilter.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.installEventFilter(new EventFilter(&a));
    MainWindow w;
    w.show();
    return a.exec();
}
