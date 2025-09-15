#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "utils.h"
#include "set.h"
#include <fstream>
#include <filesystem>
#include "serie.h"
#include <algorithm>
namespace fs = std::filesystem;

inline std::string trim(const std::string &s)
{
    auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c){return std::isspace(c);});
    auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c){return std::isspace(c);}).base();
    return (wsback<=wsfront ? std::string() : std::string(wsfront, wsback));
}

std::string GetColorString(Color c){
    switch (c){
    case Color::YELLOW:
        return "Yellow";
    case Color::BLUE:
        return "Blue";
    case Color::RED:
        return "Red";
    case Color::GREEN:
        return "Green";
    case Color::PURPLE:
        return "Purple";
    case Color::ALL:
        return "All";
    default:
        return "None";
    }
}

std::string GetTriggerString(Trigger t){
    switch (t){
        case Trigger::BAG:
            return "Goldbag";
        case Trigger::BAR:
            return "Goldbar";
        case Trigger::BOOK:
            return "Book";
        case Trigger::BURN:
            return "Burn";
        case Trigger::CHOICE:
            return "Choice";
        case Trigger::PANT:
            return "Pant";
        case Trigger::SALVAGE:
            return "Salvage";
        case Trigger::SOUL:
            return "Soul";
        case Trigger::STANDBY:
            return "Standby";
        case Trigger::WIND:
            return "Wind";
        default:
            return "None";
    }
}


std::string GetCardTypeString(CardType t){
    switch (t){
    case CardType::CHARACTER:
        return "Character";
    case CardType::EVENT:
        return "Event";
    default:
        return "Climax";
    }
}
void ParseCards(Set& set){
    std::vector<Card>& cards = set.getCards();
    std::string set_path = set.getPath();
    std::string card_data = set_path+separator()+"CardData.txt";
    if (fs::exists(card_data)){
        std::ifstream file(card_data);
        std::string str;
        CardType type = CardType::CHARACTER;
        std::string key = "";
        std::string name = "";
        std::string path = "";
        Color color = Color::NONE;
        uint level = 0;
        uint cost = 0;
        int power = 1;
        Trigger trigger1 = Trigger::NONE;
        Trigger trigger2 = Trigger::NONE;
        uint soul_count = 0;
        std::string code = "";
        std::string text = "";
        std::string trait1="";
        std::string trait2="";
        std::string trait3="";

        bool is_previous_line_trait = false;
        bool found_text = false;
        while (std::getline(file, str))
        {
            // std::cout << "reading line : " << str << std::endl;
            if (str != "" && str.substr(0,5) != "Combo"){
                if (str.substr(0,10) == "Character:"){
                    key=trim(str.substr(10,std::string::npos));
                    type = CardType::CHARACTER;
                }
                else if (str.substr(0,6) == "Event:"){
                    key=trim(str.substr(6,std::string::npos));
                    type = CardType::EVENT;
                }
                else if (str.substr(0,7) == "Climax:"){
                    key=trim(str.substr(7,std::string::npos));
                    type = CardType::CLIMAX;
                }

                else if (str.substr(0,6) == "Image "){
                    path=set_path+separator()+trim(str.substr(6,std::string::npos))+".jpg";
                }

                else if (str.substr(0,5) == "Name "){
                    name= trim(str.substr(5,std::string::npos));
                }

                else if (str.substr(0,6) == "Color "){

                    std::string temp_color = trim(str.substr(6,std::string::npos));
                    if (key == "KS/W49-T10"){
                        std::cout << "temp color : " << temp_color << std::endl;
                    }
                    if (temp_color == "Y"){
                        color = Color::YELLOW;
                    }
                    else if (temp_color == "B"){
                        color = Color::BLUE;
                    }
                    else if (temp_color == "G"){
                        color = Color::GREEN;
                    }
                    else if (temp_color == "R"){
                        color = Color::RED;
                    }
                    else {
                        // default case is NONE.
                        // right now, no case for ALL colors, but it could be possible later during effects
                        color = Color::NONE;
                    }
                }
                else if (str.substr(0,6) == "Level "){
                    unsigned long lresult = stoul(trim(str.substr(6,std::string::npos)), 0, 10);
                    unsigned int result = lresult;
                    if (result != lresult) throw std::out_of_range("Error parsing level ! ");
                    level = result;
                }
                else if (str.substr(0,5) == "Cost "){
                    unsigned long lresult = stoul(trim(str.substr(5,std::string::npos)), 0, 10);
                    unsigned int result = lresult;
                    if (result != lresult) throw std::out_of_range("Error parsing cpst ! ");
                    cost = result;
                }
                else if (str.substr(0,6) == "Power "){
                    power = std::stoi(trim(str.substr(6,std::string::npos)));
                }
                else if (str.substr(0,5) == "Trait"){
                    is_previous_line_trait = true;
                    std::string found_trait=trim(str.substr(6,std::string::npos)); // we skip 1 digit here that is the number of the trait
                    if (trait1 == ""){
                        trait1 = found_trait;
                    }
                    else if (trait2 == ""){
                        trait2 = found_trait;
                    }
                    else if (trait3 == ""){
                        trait3 = found_trait;
                    }
                }
                else if (type == CardType::CLIMAX && str.substr(0,1) == "*"){ // THIS IS ONLY FOR DETERMINING CLIMAX TRIGGERS AND COLORS. MANDATORY
                    //todo


                    std::string trigger_found = trim(str.substr(1,std::string::npos));
                    if (trigger_found == "DoorClimax"){
                        trigger1 = Trigger::SALVAGE;
                        trigger2 = Trigger::NONE;
                        color = Color::RED;
                    }

                    if (trigger_found == "StandbyClimax"){
                        trigger1 = Trigger::STANDBY;
                        trigger2 = Trigger::SOUL;
                        color = Color::RED;
                    }

                    if (trigger_found == "BookClimax"){
                        trigger1 = Trigger::BOOK;
                        trigger2 = Trigger::SOUL;
                        color = Color::BLUE;
                    }


                    if (trigger_found == "GateClimax"){
                        trigger1 = Trigger::PANT;
                        trigger2 = Trigger::SOUL;
                        color = Color::BLUE;
                    }

                    if (trigger_found == "BarClimax"){
                        trigger1 = Trigger::BAR;
                        trigger2 = Trigger::NONE;
                        color = Color::GREEN;

                    }

                    if (trigger_found == "BagClimax"){
                        trigger1 = Trigger::BAG;
                        trigger2 = Trigger::NONE;
                        color = Color::GREEN;

                    }

                    if (trigger_found == "WindClimax"){
                        trigger1 = Trigger::WIND;
                        trigger2 = Trigger::NONE;
                        color = Color::YELLOW;
                    }

                    if (trigger_found == "ChoiceClimax"){
                        trigger1 = Trigger::CHOICE;
                        trigger2 = Trigger::NONE;
                        color = Color::YELLOW;
                    }

                    if (trigger_found == "ShotClimax"){
                        trigger1 = Trigger::BURN;
                        trigger2 = Trigger::SOUL;
                        color = Color::YELLOW;
                    }

                    if (trigger_found == "BlueStockSoul"){
                        trigger1 = Trigger::SOUL;
                        trigger2 = Trigger::SOUL;
                        color = Color::BLUE;
                    }

                    if (trigger_found == "YellowStockSoul"){
                        trigger1 = Trigger::SOUL;
                        trigger2 = Trigger::SOUL;
                        color = Color::YELLOW;
                    }

                    if (trigger_found == "RedStockSoul"){
                        trigger1 = Trigger::SOUL;
                        trigger2 = Trigger::SOUL;
                        color = Color::RED;
                    }

                    if (trigger_found == "GreenStockSoul"){
                        trigger1 = Trigger::SOUL;
                        trigger2 = Trigger::SOUL;
                        color = Color::GREEN;
                    }

                    if (trigger_found == "2K2Climax(Yellow)"){
                        trigger1 = Trigger::SOUL;
                        trigger2 = Trigger::SOUL;
                        color = Color::YELLOW;
                    }

                    if (trigger_found == "2K2Climax(Red)"){
                        trigger1 = Trigger::SOUL;
                        trigger2 = Trigger::SOUL;
                        color = Color::RED;
                    }

                    if (trigger_found == "2K2Climax(Green)"){
                        trigger1 = Trigger::SOUL;
                        trigger2 = Trigger::SOUL;
                        color = Color::GREEN;
                    }

                    if (trigger_found == "2K2Climax(Blue)"){
                        trigger1 = Trigger::SOUL;
                        trigger2 = Trigger::SOUL;
                        color = Color::BLUE;
                    }

                    if (trigger_found == "3K1Climax(Red)"){
                        trigger1 = Trigger::SOUL;
                        trigger2 = Trigger::SOUL;
                        color = Color::RED;
                    }

                    if (trigger_found == "3K1Climax(Yellow)"){
                        trigger1 = Trigger::SOUL;
                        trigger2 = Trigger::SOUL;
                        color = Color::YELLOW;
                    }

                    if (trigger_found == "3K1Climax(Green)"){
                        trigger1 = Trigger::SOUL;
                        trigger2 = Trigger::SOUL;
                        color = Color::GREEN;
                    }

                    if (trigger_found == "3K1Climax(Blue)"){
                        trigger1 = Trigger::SOUL;
                        trigger2 = Trigger::SOUL;
                        color = Color::BLUE;
                    }



                    if (trigger_found == "2SoulClimax"){
                        trigger1 = Trigger::SOUL;
                        trigger2 = Trigger::SOUL;

                    }
                }
                else if ((is_previous_line_trait && str.substr(0,5) != "Trait" && !found_text && str.substr(0,5)!= "Text ") || str.substr(0,7) == "Effect:"){
                    // we finished, now everything that is after this is card code, unless found a line that begins with "Text"
                    is_previous_line_trait = true;
                    code += trim(str);
                }
                else if (((is_previous_line_trait && str.substr(0,5) == "Text ") || (found_text)) && str.substr(0,7) != "EndCard"){
                    found_text = true;
                    if (str.substr(0,5) == "Text "){
                        text += trim(str.substr(5,std::string::npos));
                    }
                    else {
                        text += trim(str);
                    }
                    // here, we finished going through the card code, it's time to parse text
                }

                else if (str.substr(0,7) == "EndCard"){
                    cards.push_back(Card(key,type,name,path,color,level,cost,power,trigger1,trigger2,soul_count,code,text,trait1,trait2,trait3)); //
                    key = "";
                    type=CardType::CHARACTER;
                    name = "";
                    path = "";
                    color = Color::NONE;
                    level = 0;
                    cost = 0;
                    power = 1;
                    trigger1 = Trigger::NONE;
                    trigger2 = Trigger::NONE;
                    soul_count = 0;
                    code = "";
                    text = "";
                    trait1="";
                    trait2="";
                    trait3="";

                    bool is_previous_line_trait = false;
                    bool found_text = false;
                }
            }
        }
    }

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
                        Set found = Set(set_key,set_name,set_folder);
                        ParseCards(found);
                        sets.push_back(found);
                    }
                }
            }
        }
    }
}

std::vector<Serie> ParseSeries(std::string cards_path){
    // list all subfolders
    std::vector<std::string> series_found = ListFoldersInFolder(std::string_view {cards_path});
    std::sort(series_found.begin(), series_found.end());
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



bool endsWith(const std::string& s, const std::string& suffix)
{
    return s.rfind(suffix) == (s.size() - suffix.size());
}

//Determine pass separator using OS
char separator()
{
#if defined _WIN32 || defined __CYGWIN__
    return '\\';
#else
    return '/';
#endif
}




// split a path, and retrieve the last part of it, split by a separator
std::string GetLastName(const std::string phrase){
    std::string last_name;
    std::string s = std::string(phrase);
    char delimiter = separator();
    size_t pos = 0;
    std::string token;
    // this part is here to continue after finding the last separator, to get the real last term
    bool break_loop {false};
    size_t prev_pos = 0;
    while ((pos == std::string::npos && !break_loop) || (pos != std::string::npos))  {
        if (pos == std::string::npos){
            // if we reached the end, only take the last part with the previous stored position
            break_loop = true;
            token = s.substr(prev_pos, s.size());
            last_name = token;
        }
        else {
            // If we're in the middle of the path, use current position
            token = s.substr(0, pos);
            last_name = token;
            s.erase(0, pos + sizeof(delimiter));
            pos = s.find(delimiter);
        }
    }
    return last_name;
}


// Parse folders inside current folder
std::vector<std::string> ListFoldersInFolder(const std::string_view folder){
    std::vector<std::string> foundFolders;
    for (const auto & entry : fs::directory_iterator(folder))
        if (entry.is_directory()){
            std::string name_dic = GetLastName(entry.path());
            if (name_dic.substr(0,1) != "."){
                foundFolders.push_back(entry.path());
            }
        }
    return foundFolders;
}

std::vector<std::string> ListFilesInFolder(const std::string_view folder,std::string extension="jpg"){
    std::vector<std::string> foundFiles;
    for (const auto & entry : fs::directory_iterator(folder))
        if (!entry.is_directory() && endsWith(entry.path(),"."+extension)){
            foundFiles.push_back(entry.path());
        }
    return foundFiles;
}


