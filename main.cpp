#include "mainwindow.h"
#include <QPushButton>
#include <QApplication>
#include <iostream>
void Test(){
    std::cout << "test test" << std::endl;
}

int main(int argc, char *argv[])
{
    // instantiate deck parser
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
