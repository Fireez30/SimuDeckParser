#include "io.h"
#include "data.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "algorithms.h"
#include "tinyxml2.h"
#include "set.h"
#include <fstream>
#include <QFile>
#include <filesystem>
#include "serie.h"
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "deck.h"
#include "HTTPRequest.hpp"
#include "json.hpp"
#include <ctime>
std::string card_to_print = "GL/S52-E068";
using json = nlohmann::json;
namespace fs = std::filesystem;
std::map<std::string,std::string> common_effects = {};

static std::string base64_encode(const std::string &in) {

    std::string out;

    int val = 0, valb = -6;
    for (uchar c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
            valb -= 6;
        }
    }
    if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
    while (out.size()%4) out.push_back('=');
    return out;
}

static std::string base64_decode(const std::string &in) {

    std::string out;

    std::vector<int> T(256,-1);
    for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val=0, valb=-8;
    for (uchar c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val>>valb)&0xFF));
            valb -= 8;
        }
    }
    return out;
}

std::vector<std::string> TransformToExistingCardKey(std::vector<Serie> &series,const std::vector<std::string> card_keys){ // OPTIMIZE THIS LATER , WANT TO DO SOME TESTS
    std::vector<std::string> final_card_list = {};
    std::vector<std::string> existing_card_codes = {};

    for (Serie s : series){ // Do not keep this in final release , too slow.
        //std::cout << "is it in serie " << s.getName() << std::endl;
        for (Set set : s.getAllSets()){
            //std::cout << "is it in set " << set.getName() << std::endl;
            std::map<std::string,Card>::iterator it;
            for (it = set.getCards().begin(); it != set.getCards().end(); it++){
                std::string code = (*it).first;
                existing_card_codes.push_back(code);
            }
        }
    }

    // NON FOIL CARD
    //std::string new_code = removeTrailingAlphas(code);
    for (std::string card_key : card_keys){
        bool found = false;
        std::string sub_raw_str;
        std::string sub_str = card_key;
        if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
            final_card_list.push_back(sub_str);
            found = true;
        }

        if (!found){
            sub_str = card_key; // check if to lower
            sub_raw_str = card_key;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }
        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-","-E");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-","-E");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-T","-TE");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-T","-TE");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-P","-PE");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }
        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-P","-PE");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            replace_in_string(sub_str,"-P","-PE");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            replace_in_string(sub_str,"-P","-PE");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            final_card_list.push_back(card_key);
        }

    }

    return final_card_list;
}

bool AddDeckToSimulator(std::string name,std::string date, std::vector<std::string> card_list){
    bool added = false;
    std::string encoded_name = base64_encode(name);
    std::string encoded_date = base64_encode(date);

    return added;
}

void ParseDeckFromJson(json deck_json,std::map<std::string,Deck> &decks,std::vector<Serie> &series,bool print){
    if (print){
        std::cout << " Parse deck from JSON" << std::endl;
        //std::cout << deck_json << std::endl;
        //for (json::iterator it = deck_json.begin(); it != deck_json.end(); ++it) {
        //    std::cout << it.key()  << "\n";
        //}

    }
    std::string deck_name;
    std::string deck_date;
    if (deck_json.contains("name")){
        deck_name = trim(deck_json["name"]);
    }
    else {
        std::cout << "name not found" << std::endl;
    }

    if (deck_json.contains("datecreated")){
        deck_date = trim(deck_json["datecreated"]);
    }
    else {
        std::cout << "datecreated not found" << std::endl;
        if (deck_json.contains("datemodified")){
            deck_date = trim(deck_json["datemodified"]);
        }
    }
    std::string final_date = "";
    //
    //  transform date
    //
    std::string trimmed = deck_date.substr(0, 19); // "2025-09-30T22:20:36"

    std::tm tm = {};
    std::istringstream ss(trimmed);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

    if (ss.fail()) {
        std::cerr << "Failed to parse time\n";
    } else {
        std::time_t time = std::mktime(&tm);
        //std::cout << "Parsed time: " << std::ctime(&time);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M %d/%m/%Y");

        final_date = oss.str();        //15:26  09/22/2025
    }

    std::vector<std::string> deck_list = {};
    int ncards = deck_json["cards"].size();
    for(auto ct=deck_json["cards"].begin();ct!=deck_json["cards"].end();++ct){
        json card_json = ct.value();
        if (print)
            std::cout << card_json << "\n";
        std::string card_code = trim(card_json["cardcode"]);
        deck_list.push_back(card_code);

        //
        // write in prefs file
        //
    }
    std::vector<std::string> new_deck_list = TransformToExistingCardKey(series,deck_list);

    std::cout << deck_name << std::endl;
    std::cout << final_date << std::endl;
    std::cout << "Deck list found " << new_deck_list.size() << " cards" << std::endl;
    for (std::string card : new_deck_list){
        std::cout << card << std::endl;
    }

    return;
    bool added = AddDeckToSimulator(deck_name,final_date,new_deck_list);
    if (added){
        decks.insert_or_assign(deck_name, *(new Deck(deck_name,deck_date)));
        LoadDeckFromList(decks,series,new_deck_list,deck_name);
    }
    else {
        std::cout << "ERROR ADDING TO SIMULATOR" << std::endl;
    }
}


json SendRequest(std::string url){

    // you can pass http::InternetProtocol::V6 to Request to make an IPv6 request
    http::Request request{url};

    // send a get request
    std::cout << url << "\n";
    const auto response = request.send("GET");
    std::string resp =  std::string{response.body.begin(), response.body.end()};
    json j1 = json::parse(resp);
    return j1;

}

void ParseDeckById(std::string deck_code,std::map<std::string,Deck> &decks,std::vector<Serie> &series){
    std::cout << "parsing deck " << deck_code << std::endl;
    json deckobject = SendRequest("http://www.encoredecks.com/api/deck/"+trim(deck_code));
    ParseDeckFromJson(deckobject,decks,series,false);
}



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

}

std::string retrieveCardPath(const std::string set_path,const std::string card_key,const std::string alternate_artwork_folder){ // OPTIMIZE THIS LATER , WANT TO DO SOME TESTS
    // NON FOIL CARD
    char separator_found = separator();
    std::string sub_str = card_key;
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    std::string path=set_path+separator_found+trim(sub_str)+".jpg";

    if (card_key == card_to_print){ std::cout << "trying path : " << path << std::endl;}

    if (QFile::exists(QString::fromStdString(path))) { // Normal card found
        return path;
    }

    sub_str = card_key; // check if to lower
    transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
              ::tolower);
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    path=set_path+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Normal card transformed for English
        return path;
    }


    sub_str = card_key;
    replace_in_string(sub_str,"-","-E");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    path=set_path+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Normal card transformed for English
        return path;
    }

    sub_str = card_key;
    replace_in_string(sub_str,"-","-E");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    path=set_path+separator_found+trim(sub_str)+".jpg";
    transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
              ::tolower);
    if (QFile::exists(QString::fromStdString(path))) { // Normal card transformed for English to lower
        return path;
    }


    sub_str = card_key;
    replace_in_string(sub_str,"-T","-TE");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    path=set_path+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Trial deck card transformed for English
        return path;
    }

    sub_str = card_key;
    replace_in_string(sub_str,"-T","-TE");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
              ::tolower);
    path=set_path+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Trial deck card transformed for English to lower
        return path;
    }

    sub_str = card_key;
    replace_in_string(sub_str,"-P","-PE");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    path=set_path+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Promo card transformed for English
        return path;
    }

    sub_str = card_key;
    replace_in_string(sub_str,"-P","-PE");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
              ::tolower);
    path=set_path+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Promo card transformed for English
        return path;
    }

    // FOIL CARD (JP)

    //ARI/S103-001 => jap folder : S103
    std::string foil_set = "";
    std::vector<std::string> splitted_slash = split(card_key,'/');
    if (splitted_slash.size() > 1){
        std::vector<std::string> splitted_dash = split(splitted_slash.at(1),'-');
        if (splitted_dash.size() > 1){
            foil_set = splitted_dash[0];
        }
    }


    sub_str = card_key;
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');

    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";

    if (QFile::exists(QString::fromStdString(path))) { // Normal card found
        return path;
    }


    sub_str = card_key;
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
              ::tolower);
    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";

    if (QFile::exists(QString::fromStdString(path))) { // Normal card found to lower
        return path;
    }

    sub_str = card_key;
    replace_in_string(sub_str,"-","-E");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Normal card transformed for English
        return path;
    }

    sub_str = card_key;
    replace_in_string(sub_str,"-","-E");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
              ::tolower);
    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Normal card transformed for English to lower
        return path;
    }

    sub_str = card_key;
    replace_in_string(sub_str,"-T","-TE");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Trial deck card transformed for English
        return path;
    }
    sub_str = card_key;
    replace_in_string(sub_str,"-T","-TE");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
              ::tolower);
    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Trial deck card transformed for English to lower
        return path;
    }

    sub_str = card_key;
    replace_in_string(sub_str,"-P","-PE");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Promo card transformed for English
        return path;
    }

    sub_str = card_key;
    replace_in_string(sub_str,"-P","-PE");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
              ::tolower);
    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Promo card transformed for English to lower
        return path;
    }


    //ARI/S103-001 => en folder : ENS103

    foil_set = "EN"+foil_set;
    sub_str = card_key;
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');

    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";

    if (QFile::exists(QString::fromStdString(path))) { // Normal card found
        return path;
    }

    sub_str = card_key;
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
              ::tolower);
    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";

    if (QFile::exists(QString::fromStdString(path))) { // Normal card found to lower
        return path;
    }

    sub_str = card_key;
    replace_in_string(sub_str,"-","-E");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Normal card transformed for English
        return path;
    }

    sub_str = card_key;
    replace_in_string(sub_str,"-","-E");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
              ::tolower);
    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Normal card transformed for English to lower
        return path;
    }

    sub_str = card_key;
    replace_in_string(sub_str,"-T","-TE");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Trial deck card transformed for English
        return path;
    }

    sub_str = card_key;
    replace_in_string(sub_str,"-T","-TE");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
              ::tolower);
    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Trial deck card transformed for English to lower
        return path;
    }

    sub_str = card_key;
    replace_in_string(sub_str,"-P","-PE");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Promo card transformed for English
        return path;
    }


    sub_str = card_key;
    replace_in_string(sub_str,"-P","-PE");
    std::replace(sub_str.begin(), sub_str.end(), '/', '_');
    std::replace(sub_str.begin(), sub_str.end(), '-', '_');
    transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
              ::tolower);
    path=alternate_artwork_folder+separator_found+foil_set+separator_found+trim(sub_str)+".jpg";
    if (QFile::exists(QString::fromStdString(path))) { // Promo card transformed for English to lower
        return path;
    }

    return ":/images/emptycard.jpg";
}
void ParseCards(Set& set,std::string alternate_cards_folder){
    std::map<std::string,Card>& cards = set.getCards();
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
                    if (key == card_to_print){std::cout << "Found character for key " << card_to_print << std::endl;}
                    type = CardType::CHARACTER;
                }
                else if (str.substr(0,6) == "Event:"){
                    key=trim(str.substr(6,std::string::npos));
                    if (key == card_to_print){std::cout << "Found event for key " << card_to_print << std::endl;}
                    type = CardType::EVENT;
                }
                else if (str.substr(0,7) == "Climax:"){
                    key=trim(str.substr(7,std::string::npos));
                    if (key == card_to_print){std::cout << "Found climax for key " << card_to_print << std::endl;}
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

                    if (key == card_to_print){std::cout << "Found path " << path << " for key " << card_to_print << std::endl;}

                }

                else if (str.substr(0,5) == "Name "){
                    name= trim(str.substr(5,std::string::npos));
                    if (key == card_to_print){std::cout << "Found name " << name << " for key " << card_to_print << std::endl;}
                }

                else if (str.substr(0,6) == "Color "){

                    std::string temp_color = trim(str.substr(6,std::string::npos));
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
                    if (key == card_to_print){std::cout << "Found color " << GetColorString(color) << " for key " << card_to_print << std::endl;}

                }
                else if (str.substr(0,6) == "Level "){
                    unsigned long lresult = stoul(trim(str.substr(6,std::string::npos)), 0, 10);
                    unsigned int result = lresult;
                    if (result != lresult) throw std::out_of_range("Error parsing level ! ");
                    level = result;
                    if (key == card_to_print){std::cout << "Found level " << level << " for key " << card_to_print << std::endl;}

                }
                else if (str.substr(0,6) ==  "Clone "){
                    std::string cloned_card_name = trim(str.substr(6,std::string::npos));
                    if (key == card_to_print){std::cout << "Found clone " << cloned_card_name << " for key " << card_to_print << std::endl;}

                    //std::cout << key << " cloned from " << cloned_card_name << std::endl;
                    std::map<std::string,Card>::iterator it;
                    for (it = cards.begin(); it != cards.end(); it++){
                        std::string code = (*it).first;
                        Card c  = (*it).second;
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
                    if (key == card_to_print){std::cout << "Found cost " << cost << " for key " << card_to_print << std::endl;}

                }
                else if (str.substr(0,6) == "Power "){
                    power = std::stoi(trim(str.substr(6,std::string::npos)));
                    if (key == card_to_print){std::cout << "Found power " << power << " for key " << card_to_print << std::endl;}

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
                    if (key == card_to_print){std::cout << "Found trait1 " << trait1 << " for key " << card_to_print << std::endl;}
                        if (key == card_to_print){std::cout << "Found trait2 " << trait2 << " for key " << card_to_print << std::endl;}
                        if (key == card_to_print){std::cout << "Found trait3 " << trait3 << " for key " << card_to_print << std::endl;}
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

                    if (key == card_to_print){std::cout << "Found triggr1 " << GetTriggerString(trigger1) << " for key " << card_to_print << std::endl;}
                        if (key == card_to_print){std::cout << "Found trait2 " << GetTriggerString(trigger2) << " for key " << card_to_print << std::endl;}
                        if (key == card_to_print){std::cout << "Found color " << GetColorString(color) << " for key " << card_to_print << std::endl;}
                }
                else if (str.substr(0,1) == "*"){
                    std::string effect_parsed = trim(str.substr(1,std::string::npos));
                    replace_in_string(effect_parsed,"()","");
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
                        std::map<std::string,std::string>::iterator it;
                        for (it = common_effects.begin(); it != common_effects.end(); it++){
                            if (!found_effect){

                                std::string effect_key = trim((*it).first);
                                std::string effect_text = trim((*it).second);

                                std::string preffix_effect_key = effect_key.substr(0,effect_key.find('('));
                                std::string prefix_str_key = effect_parsed.substr(0,effect_parsed.find('('));
                                if (preffix_effect_key == prefix_str_key){
                                    if (key == card_to_print){std::cout << "before substr" << std::endl;}
                                    if (effect_key.find('(') != std::string::npos && effect_parsed.find('(') != std::string::npos){

                                        std::string params_effect_key = effect_key.substr(effect_key.find('('),std::string::npos);
                                        std::string params_str_key  = effect_parsed.substr(effect_parsed.find('('),std::string::npos);
                                        replace_in_string(params_effect_key,"(","");
                                        replace_in_string(params_effect_key,")","");
                                        replace_in_string(params_str_key,"(","");
                                        replace_in_string(params_str_key,")","");
                                        std::vector<std::string> list_of_params_effect_key = split(params_effect_key,',');
                                        std::vector<std::string> list_of_params_str_key = split(params_str_key,',');



                                        found_effect = true;
                                        used = true;
                                        if (list_of_params_effect_key.size() == list_of_params_str_key.size()){
                                            if (list_of_params_str_key.size() > 0){
                                                for (int i {0}; i < list_of_params_effect_key.size(); ++i){
                                                    replace_in_string(effect_text,trim(list_of_params_effect_key[i]),trim(list_of_params_str_key[i]));
                                                }
                                                replace_in_string(effect_text,"$","");
                                                replace_in_string(effect_text,"|"," or ");
                                            }
                                            text += trim(effect_text) + "\n";
                                            if (key == card_to_print){std::cout << "Append to text " << trim(effect_text) << " for key " << card_to_print << std::endl;}
                                            break;
                                        }
                                    }
                                }
                            }
                        }

                    }
                    if (!used){
                        text += trim(str.substr(1,std::string::npos)) + "\n" ; // Here we replace common effect tag by the real text
                        if (key == card_to_print){std::cout << "Append to text " << trim(str.substr(1,std::string::npos)) << " for key " << card_to_print << std::endl;}

                    }

                }
                else if ((str.substr(0,7) != "EndCard" && str.substr(0,5) != "Trait" && str.substr(0,5)!= "Text ") && (parsing_code || str.substr(0,5) == "Cont:" || str.substr(0,5) == "Auto:" || str.substr(0,4) == "Act:")){
                    // we finished, now everything that is after this is card code, unless found a line that begins with "Text"
                    is_previous_line_trait = true;
                    if (parsing_text){
                        // append text;
                        parsing_text = false;
                    }
                    parsing_code = true;
                    if (key == card_to_print){std::cout << "Append to code " << trim(str) << " for key " << card_to_print << std::endl;}

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
                        if (key == card_to_print){std::cout << "Append to text " << trim(str.substr(5,std::string::npos)) << " for key " << card_to_print << std::endl;}

                        text += trim(str.substr(5,std::string::npos)) + "\n";
                    }
                    else {
                            if (key == card_to_print){std::cout << "Append to text " << trim(str) << " for key " << card_to_print << std::endl;}

                        text += trim(str) + "\n" ;
                    }
                    // here, we finished going through the card code, it's time to parse text
                }

                else if (str.substr(0,7) == "EndCard"){
                    path = retrieveCardPath(set_path,key,alternate_cards_folder);
                    //std::cout << "found path for card " << key << " = " << path << std::endl;

                    cards[key] = Card(key,type,name,path,color,level,cost,power,trigger1,trigger2,soul_count,code,text,trait1,trait2,trait3); //
                    if (key == card_to_print){
                        cards[key].print();
                    }
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

    std::string alternate_artwork_path = card_path;
    replace_in_string(alternate_artwork_path,"Cards","AlternateArtwork");
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
                        ParseCards(found,alternate_artwork_path);
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



void ParseDecks(std::map<std::string,Deck> &decks){
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    std::string homedirstr = homedir;
    std::string prefsfile = homedirstr+separator()+".config"+separator()+"unity3d"+separator()+"Blake Thoennes"+separator()+"Weiss Schwarz"+separator()+"prefs";
    if (fs::exists(prefsfile)){
        tinyxml2::XMLDocument doc;
        doc.LoadFile(prefsfile.c_str());
        tinyxml2::XMLElement* rootnode = doc.RootElement();
        tinyxml2::XMLElement* firstchild = rootnode->FirstChildElement();
        while (firstchild != nullptr){
            if (firstchild->FindAttribute("name") != nullptr){
            std::string name = firstchild->FindAttribute("name")->Value();
                if (name.substr(0,5) == "Date_"){
                    std::string deck_name = trim(name.substr(5,std::string::npos));
                    std::string base64_date = firstchild->GetText();
                    std::string deck_date = trim(base64_decode(base64_date));
                    decks.insert_or_assign(deck_name, *(new Deck(deck_name,deck_date)));
                }
            }
            firstchild = firstchild->NextSiblingElement();
        }
    }
}
void LoadDeckFromList(std::map<std::string,Deck> &decks,std::vector<Serie> &series,std::vector<std::string> card_list,std::string deck_name){
    std::map<std::string,Deck>::iterator itdeck;
    itdeck = decks.find (deck_name);
    //std::cout << "after declaration of iterator" << std::endl;
    if (itdeck != decks.end()){
        std::cout << "card codes length : " << card_list.size()  << std::endl;
        //std::cout << "after getting codes" << std::endl;
        for (std::string card : card_list){
            bool found = false;
            //std::cout << "card " << card << std::endl;
            if (trim(card) != ""){
                for (Serie s : series){ // Do not keep this in final release , too slow.
                    //std::cout << "is it in serie " << s.getName() << std::endl;
                    if (!found){
                        for (Set set : s.getAllSets()){
                            //std::cout << "is it in set " << set.getName() << std::endl;
                            if (!found && set.containsCard(card)){
                                Card* c = set.getCard(card);
                                //std::cout << "retrieved card" << std::endl;
                                (*itdeck).second.AddCard(*c);
                                //std::cout << "added card" << std::endl;
                                found = true;
                            }
                        }
                    }
                }
            }
            if (!found){
                std::cout << " Didn't find anything related to : " << card << std::endl;
            }
        }

    }
    std::cout << "finished loading deck , loaded : " << (*itdeck).second.getCardList().size() << " card " << std::endl;
}
void LoadDeck(std::map<std::string,Deck> &decks, std::string target_deck_name,std::vector<Serie> &series)
{

    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    std::string homedirstr = homedir;
    std::string prefsfile = homedirstr+separator()+".config"+separator()+"unity3d"+separator()+"Blake Thoennes"+separator()+"Weiss Schwarz"+separator()+"prefs";
    //std::cout << "pref file : " << prefsfile << std::endl;
    if (fs::exists(prefsfile)){
        //std::cout << "Found pref file : "<< std::endl;
        tinyxml2::XMLDocument doc;
        doc.LoadFile(prefsfile.c_str());
        tinyxml2::XMLElement* rootnode = doc.RootElement();
        //std::cout << "Root node : " << rootnode->Name() << std::endl;
        tinyxml2::XMLElement* firstchild = rootnode->FirstChildElement();
        while (firstchild != nullptr){
            //std::cout << "found child : " << std::endl;
            if (firstchild->FindAttribute("name") != nullptr){
                //std::cout << firstchild->FindAttribute("name")->Value() << std::endl;
                std::string name = firstchild->FindAttribute("name")->Value();
                if (name.substr(0,5) == "Deck_"){
                    //std::cout << "found deck " << std::endl;
                    std::string deck_name = name.substr(5,std::string::npos);
                    if (deck_name == target_deck_name){
                        //std::cout << "found date name " << std::endl;
                        //std::cout << "found deck : " << deck_name << std::endl;
                        // tinyxml2::XMLText* textNode = firstchild->ToText();
                        //std::cout << "found textNode " << std::endl;

                        ///std::cout << "after while" << std::endl;
                        std::string base64_decklist = firstchild->GetText();
                        //std::cout << "found base64_decklist " << std::endl;
                        std::string deck_list = trim(base64_decode(base64_decklist));
                        //std::cout <<  " list : " << deck_list << std::endl;
                        std::vector<std::string> card_codes  = split(deck_list,'|');
                        LoadDeckFromList(decks,series,card_codes,deck_name);

                    }
                }
            }
            firstchild = firstchild->NextSiblingElement();
        }
    }

}

