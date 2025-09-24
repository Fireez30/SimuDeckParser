#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>

#include "utils.h"
class Card
{
public:
    Card();
    Card(std::string key,CardType type,std::string name,std::string path,Color color,uint level,uint cost, int power,Trigger trigger1, Trigger trigger2,uint soul_count,std::string code, std::string text,std::string trait1="",std::string trait2="",std::string trait3="");
    std::string getKey();
    std::string getName();
    std::string getImagePath();
    Color getColor();
    uint getLevel();
    uint getCost();
    int getPower();
    std::vector<Trigger> getTriggers();
    uint getSoulCount();
    std::vector<std::string> getTraits();
    std::string getSimulatorCode();
    CardType getCardType();
    std::string getText();
    void print();
    std::string getWholeCardText();
protected:
    CardType type;
    std::string key;
    std::string name;
    std::string image_path;
    Color color;
    uint level;
    uint cost;
    int power;
    std::vector<Trigger> triggers;
    uint soul_count;
    std::vector<std::string> traits;
    std::string simulator_code;
    std::string text;
};

#endif // CARD_H
