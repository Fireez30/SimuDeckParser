#include "mainwindow.h"
#include <iostream>
#include "utils.h"
#include <QApplication>
#include "serie.h"
#include "set.h"
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

inline std::string trim(const std::string &s)
{
    auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c){return std::isspace(c);});
    auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c){return std::isspace(c);}).base();
    return (wsback<=wsfront ? std::string() : std::string(wsfront, wsback));
}

void ParseSets(Serie& serie,std::string card_path){
    std::string serie_path = serie.getPath();
    std::vector<Set>& sets = serie.getAllSets();
    std::string single_card_data = serie_path+separator()+"SingleSetData.txt";
    if (fs::exists(single_card_data)){
        std::ifstream file(single_card_data);
        std::string str;
        bool parsing_set = false;
        std::string set_name;
        std::string set_key;
        std::string set_folder;
        //std::cout << "Serie : " << serie.getName() << std::endl;
        while (std::getline(file, str))
        {
            // std::cout << "reading line : " << str << std::endl;
            if (str != ""){
                if (str.substr(0,7) == "Folder:"){
                    std::string folder=trim(str.substr(7,std::string::npos));
                    //std::cout << "found folder : " << folder << std::endl;
                    set_folder = card_path+separator()+folder;
                }

                else if (str.substr(0,7) == "Prefix:"){
                    std::string prefix=trim(str.substr(7,std::string::npos));
                    //std::cout << "found prefix : " << prefix << std::endl;
                    set_key = prefix;
                }

                else if (str.substr(0,8) == "SetName:"){
                    std::string name=trim(str.substr(8,std::string::npos));
                    //std::cout << "found name : " << name << std::endl;
                    set_name = name;


                    //std::cout << "found serie" << std::endl;
                    if (set_name != ""){
                        std::cout << "set to add : " << set_name << std::endl;
                        sets.push_back(Set(set_name,set_key,set_folder));
                    }
                }
            }
        }
    }
}

std::vector<Serie> ParseSeries(std::string cards_path){
    // list all subfolders
    std::vector<std::string> series_found = ListFoldersInFolder(std::string_view {cards_path});
    std::vector<Serie> series {};
    for (std::string serie_path : series_found){
        // retrieve name of the subfolder as serie name
        std::string serie_name = GetLastName(serie_path);
        // Serie objects will remain along all the execution!
        Serie s = Serie(serie_name,serie_path);
        ParseSets(s,cards_path);
        series.push_back(s);

    }


    return series;
}


int main(int argc, char *argv[])
{
    // instantiate deck parser

    // input path
    std::string simulator_path { "/home/ben/Games/Weiss Schwarz 0.6.3.2 Linux/Weiss Schwarz 0.6.3.2 Linux_Data/" };
    // internal subfolders
    std::string cards_folder = simulator_path+"StreamingAssets"+separator()+"Cards"+separator();
    // parsing all series
    std::vector<Serie> series = ParseSeries(cards_folder);


    for (Serie sa : series){
        std::cout << sa.getName() << " : " << sa.getAllSets().size() << std::endl;
        for (Set set_obj : sa.getAllSets()){
            std::cout << set_obj.getName() << " , " << set_obj.getKey() << " : " << set_obj.getPath() << std::endl;
        }
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
