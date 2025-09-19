#ifndef UTILS_H
#define UTILS_H
#include<vector>
#include<string>
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
