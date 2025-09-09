#include "mainwindow.h"
#include <iostream>
#include "utils.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    std::string_view cards { "/home/ben/Games/Weiss Schwarz 0.6.3.2 Linux/Weiss Schwarz 0.6.3.2 Linux_Data/StreamingAssets/Cards/" };
    std::vector<std::string> series = ListFoldersInFolder(cards);
    for (std::string serie_path : series){
        std::string serie_name = getLastName(serie_path);
        std::cout << serie_name << std::endl;
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
