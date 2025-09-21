#ifndef UTILS_H
#define UTILS_H
#include<vector>
#include<string>
#include <map>
class Card;
class Serie;

enum class Color {
    YELLOW,
    BLUE,
    RED,
    GREEN,
    PURPLE,
    ALL,
    NONE
};

enum class CardType {
    CHARACTER,
    EVENT,
    CLIMAX
};

enum class Trigger {
    SOUL,
    CHOICE,
    BURN,
    WIND,
    STANDBY,
    SALVAGE,
    BOOK,
    PANT,
    BAR,
    BAG,
    NONE

};

enum class Orders {
    LEVEL_ASCENDING,
    LEVEL_DESCENDING,
    COST_ASCENDING,
    COST_DESCENDING,
    POWER_ASCENDING,
    POWER_DESCENDING
};


std::string GetColorString(Color c);
std::string GetCardTypeString(CardType t);
std::string GetTriggerString(Trigger t);

std::map<std::string,std::string> get_settings();
void AddOrUpdateSetting(std::string setting_key,std::string setting_value);
void WriteSettings(std::map<std::string,std::string> setting_map);
void DeleteSetting(std::string setting_key);
std::string GetSetting(std::string setting_key);
void WriteTextToFile(std::string text, std::string file_path);
void WriteCardsToFile(std::vector<Card> cards,std::string file_path);


// Parse folders inside current folder
std::vector<std::string> ListFoldersInFolder(const std::string_view folder);

//Determine pass separator using OS
char separator();

// split a path, and retrieve the last part of it, split by a separator
std::string GetLastName(const std::string phrase);

std::vector<Serie> ParseSeries(std::string cards_path);
#endif // UTILS_H
