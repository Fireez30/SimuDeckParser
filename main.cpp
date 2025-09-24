#include "mainwindow.h"
#include <QPushButton>
#include <QApplication>



int main(int argc, char *argv[])
{
    // instantiate deck parser

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
