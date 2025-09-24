#include "io.h"
#include "data.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "algorithms.h"
#include "set.h"
#include <fstream>
#include <QFile>
#include <filesystem>
#include "serie.h"
#include <algorithm>

namespace fs = std::filesystem;
std::map<std::string,std::string> common_effects = {};

void ParseCommonEffects(std::string simulator_path) {
    std::string common_effect_path = simulator_path+"StreamingAssets"+separator()+"CommonEffects(copy).txt";
    if (fs::exists(common_effect_path)){
        std::ifstream file(common_effect_path);
        std::string line;
        std::string key;
        std::string value;
        bool found_key = false;
        bool reading_value = false;
        while (std::getline(file, line))
        {
            if (line != "" && line.substr(0,2) != "//" && trim(line) != "Quick"){
                if (trim(line).substr(0,7) == "Define:"){
                    if (key != "" && value != ""){
                        common_effects[key] = value;
                        key = "";
                        value = "";
                        found_key = false;
                        reading_value = false;
                    }
                    key = trim(line.substr(8,std::string::npos));
                    found_key = true;
                }
                else if(found_key && trim(line).substr(0,5) == "Text "){
                    reading_value = true;
                    value = trim(line.substr(5,std::string::npos));
                }
                else if (found_key && reading_value && !(trim(line).substr(0,4) == "Tag " || trim(line).substr(0,4) == "Act:" || trim(line).substr(0,5) == "Auto:" || trim(line).substr(0,5) == "Cont:")){
                    value += trim(line);
                }
            }
        }
    }

    std::map<std::string,std::string>::iterator it;
    for (it = common_effects.begin(); it != common_effects.end(); it++){
        std::cout << (*it).first << std::endl;
        std::cout << (*it).second << std::endl;
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
        bool parsing_code = false;
        bool parsing_text = false;
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

                    std::string sub_str = str.substr(6,std::string::npos);
                    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
                    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
                    path=set_path+separator()+trim(sub_str)+".jpg";
                    if (!QFile::exists(QString::fromStdString(path))) {
                        path = ":/images/emptycard.jpg";
                    }

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
                else if (str.substr(0,6) ==  "Clone "){
                    std::string cloned_card_name = trim(str.substr(6,std::string::npos));
                    //std::cout << key << " cloned from " << cloned_card_name << std::endl;
                    for (Card c : cards){

                        if (c.getKey() == cloned_card_name){
                            //std::cout << "found cloned card" << c.getKey() << std::endl;
                            type=c.getCardType();
                            name = c.getName();
                            path = c.getImagePath();
                            color = c.getColor();
                            level = c.getLevel();
                            cost = c.getCost();
                            power = c.getPower();
                            trigger1 = Trigger::NONE;
                            if (c.getTriggers().size()>0){
                                trigger1 = c.getTriggers().at(0);
                            }
                            trigger2 = Trigger::NONE;
                            if (c.getTriggers().size()>1){
                                trigger2 = c.getTriggers().at(1);
                            }

                            soul_count = c.getSoulCount();
                            code = c.getSimulatorCode();
                            text = c.getText();
                            for (std::string trait : c.getTraits()){
                                if (trait1 == ""){
                                    trait1 = trait;
                                }
                                else if (trait2 == ""){
                                    trait2 = trait;
                                }
                                else if (trait3 == ""){
                                    trait3 = trait;
                                }
                            }
                        }
                    }
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
                else if (str.substr(0,1) == "*"){
                    std::string card_to_print = "KS/W76-E103";
                    std::string effect_parsed = trim(str.substr(1,std::string::npos));
                    replace_in_string(effect_parsed,"()","");
                    if (key == card_to_print){std::cout << "found start" << std::endl;}
                    std::map<std::string,std::string>::iterator it;
                    it = common_effects.find (effect_parsed);
                    bool used=false;
                    if (it != common_effects.end()){
                        if (key == card_to_print){std::cout << "found key as is " << std::endl;}
                        text += trim(it->second) + "\n";
                        used = true;
                        if (key == card_to_print){std::cout << "stored as is key" << std::endl;}
                    }
                    if (!used){
                        bool found_effect = false;
                        if (key == card_to_print){std::cout << "start loop" << std::endl;}
                        std::map<std::string,std::string>::iterator it;
                        for (it = common_effects.begin(); it != common_effects.end(); it++){
                            if (!found_effect){

                                std::string effect_key = trim((*it).first);
                                std::string effect_text = trim((*it).second);

                                std::string preffix_effect_key = effect_key.substr(0,effect_key.find('('));
                                std::string prefix_str_key = effect_parsed.substr(0,effect_parsed.find('('));
                                if (key == card_to_print){std::cout << prefix_str_key << " - " << preffix_effect_key << std::endl;}
                                if (preffix_effect_key == prefix_str_key){
                                    if (key == card_to_print){std::cout << "before substr" << std::endl;}
                                    if (effect_key.find('(') != std::string::npos && effect_parsed.find('(') != std::string::npos){

                                        std::string params_effect_key = effect_key.substr(effect_key.find('('),std::string::npos);
                                        std::string params_str_key  = effect_parsed.substr(effect_parsed.find('('),std::string::npos);
                                        replace_in_string(params_effect_key,"(","");
                                        replace_in_string(params_effect_key,")","");
                                        replace_in_string(params_str_key,"(","");
                                        replace_in_string(params_str_key,")","");
                                        if (key == card_to_print){std::cout << "before split " << std::endl;}
                                         if (key == card_to_print){std::cout << effect_key << std::endl;}
                                          if (key == card_to_print){std::cout << effect_parsed << std::endl;}
                                        std::vector<std::string> list_of_params_effect_key = split(params_effect_key,',');
                                        std::vector<std::string> list_of_params_str_key = split(params_str_key,',');


                                        if (key == card_to_print){
                                            std::cout << "list of effect split" << std::endl;
                                            for (std::string s : list_of_params_effect_key){
                                                std::cout << s << std::endl;
                                            }
                                        }

                                        if (key == card_to_print){
                                            std::cout << "list of str split" << std::endl;
                                            for (std::string s : list_of_params_str_key){
                                                std::cout << s << std::endl;
                                            }
                                        }
                                        found_effect = true;
                                        used = true;
                                        if (list_of_params_effect_key.size() == list_of_params_str_key.size()){
                                            if (list_of_params_str_key.size() > 0){
                                                for (int i {0}; i < list_of_params_effect_key.size(); ++i){
                                                    if (key == card_to_print){std::cout << "Replace " << trim(list_of_params_effect_key[i]) << " for "  << trim(list_of_params_str_key[i])<< std::endl;}
                                                    replace_in_string(effect_text,trim(list_of_params_effect_key[i]),trim(list_of_params_str_key[i]));
                                                }
                                                replace_in_string(effect_text,"$","");
                                                replace_in_string(effect_text,"|"," or ");
                                            }
                                            text += trim(effect_text) + "\n";
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        if (key == card_to_print){std::cout << "went through loop and found : " << used << std::endl;}

                    }
                    if (!used){
                        if (key == card_to_print){std::cout << "not found anything , giving back START" << std::endl;}
                        text += trim(str.substr(1,std::string::npos)) + "\n" ; // Here we replace common effect tag by the real text
                        if (key == card_to_print){std::cout << "not found anything , giving back END" << std::endl;}
                    }

                }
                else if ((str.substr(0,5) != "Trait" && str.substr(0,5)!= "Text ") && (parsing_code || str.substr(0,5) == "Cont:" || str.substr(0,5) == "Auto:" || str.substr(0,4) == "Act:")){
                    // we finished, now everything that is after this is card code, unless found a line that begins with "Text"
                    is_previous_line_trait = true;
                    if (parsing_text){
                        // append text;
                        parsing_text = false;
                    }
                    parsing_code = true;
                    code += trim(str) + "\n";
                }
                else if ((str.substr(0,7) != "EndCard" && str.substr(0,5) != "Cont:" &&  str.substr(0,5) != "Auto:" && str.substr(0,4) != "Act:") &&  (str.substr(0,5) == "Text " || parsing_text)){
                    found_text = true;
                    if (parsing_code){
                        // append text;
                        parsing_code = false;
                    }
                    parsing_text = true;
                    if (str.substr(0,5) == "Text "){
                        text += trim(str.substr(5,std::string::npos)) + "\n";
                    }
                    else {
                        text += trim(str) + "\n" ;
                    }
                    // here, we finished going through the card code, it's time to parse text
                }

                else if (str.substr(0,7) == "EndCard"){
                    std::string sub_str = key;
                    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
                    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
                    path=set_path+separator()+trim(sub_str)+".jpg";
                    if (!QFile::exists(QString::fromStdString(path))) {
                        path = ":/images/emptycard.jpg";
                    }
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

                    is_previous_line_trait = false;
                    found_text = false;
                    parsing_code = false;
                    parsing_text = false;
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
