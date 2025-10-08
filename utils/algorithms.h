#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include<string>
#include<vector>
#include "../mainwindow.h"
#include <QLayout>
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
    COLOR,
    TYPE_ASCENDING,
    LEVEL_ASCENDING,
    LEVEL_DESCENDING,
    COST_ASCENDING,
    COST_DESCENDING,
    POWER_ASCENDING,
    POWER_DESCENDING,
    KEYCODE_ASCENDING,
};


std::string GetColorString(Color c);
Color GetStringColor(std::string c);
std::string GetCardTypeString(CardType t);
CardType GetStringCardType(std::string t);
std::string GetTriggerString(Trigger t);
std::string GetTriggerPath(Trigger t);
MainWindow* getMainWindow();

//Determine pass separator using OS
std::string base64_encode(const std::string& input);
std::string base64_decode(const std::string &in);
QColor GetQColorFromCardColor(Color c);
char separator();
bool endsWith(const std::string& s, const std::string& suffix);
std::string trim(const std::string &s);
void replace_in_string(std::string &str, const std::string &a, const std::string &b);
std::vector<std::string> split(const std::string &s, char delimiter);
bool findSubstringIgnoreCase(const std::string& X, const std::string& Y);
std::string removeTrailingAlphas(const std::string& input);
std::vector<std::string> TransformToExistingCardKey(std::vector<Serie*> &series,const std::vector<std::string> card_keys);
void clearLayout(QLayout* layout);
void SortFilteredCards(std::vector<Card>* cards_to_sort,std::vector<Orders> order);
#endif // ALGORITHMS_H
