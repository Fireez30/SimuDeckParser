#include "mainwindow.h"
#include <iostream>
#include "utils.h"
#include <QApplication>
#include "serie.h"

std::vector<Serie> ParseSeries(std::string cards_path){
    std::vector<std::string> series_found = ListFoldersInFolder(std::string_view {cards_path});
    std::vector<Serie> series {};
    for (std::string serie_path : series_found){
        std::string serie_name = GetLastName(serie_path);
        if (serie_name.substr(0,1) != "."){
            Serie s = Serie(serie_name,serie_path);
            series.push_back(s);
        }
    }

    for (Serie serie_object : series){
        std::cout << serie_object.getName() << std::endl;
        std::cout << serie_object.getPath() << std::endl;
    }

    return series;
}
int main(int argc, char *argv[])
{
    std::string simulator_path { "/home/ben/Games/Weiss Schwarz 0.6.3.2 Linux/Weiss Schwarz 0.6.3.2 Linux_Data/" };
    std::string cards_folder = simulator_path+separator()+"StreamingAssets"+separator()+"Cards"+separator();
    std::vector<Serie> series = ParseSeries(cards_folder);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
