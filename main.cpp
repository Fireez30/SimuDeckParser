#include "mainwindow.h"
#include <iostream>
#include "utils.h"
#include <QApplication>
#include "serie.h"

std::vector<Serie> ParseSeries(std::string cards_path){
    // list all subfolders
    std::vector<std::string> series_found = ListFoldersInFolder(std::string_view {cards_path});
    std::vector<Serie> series {};
    for (std::string serie_path : series_found){
        // retrieve name of the subfolder as serie name
        std::string serie_name = GetLastName(serie_path);
        if (serie_name.substr(0,1) != "."){
            // Serie objects will remain along all the execution!
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
    // instantiate deck parser

    // input path
    std::string simulator_path { "/home/ben/Games/Weiss Schwarz 0.6.3.2 Linux/Weiss Schwarz 0.6.3.2 Linux_Data/" };
    // internal subfolders
    std::string cards_folder = simulator_path+separator()+"StreamingAssets"+separator()+"Cards"+separator();
    // parsing all series
    std::vector<Serie> series = ParseSeries(cards_folder);
    // for each serie
    //      Parse all sets
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
