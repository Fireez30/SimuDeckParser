#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include<string>
#include<vector>
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



//Determine pass separator using OS
char separator();
bool endsWith(const std::string& s, const std::string& suffix);
std::string trim(const std::string &s);
void replace_in_string(std::string &str, const std::string &a, const std::string &b);
std::vector<std::string> split(const std::string &s, char delimiter);
bool findSubstringIgnoreCase(const std::string& X, const std::string& Y);
#endif // ALGORITHMS_H
