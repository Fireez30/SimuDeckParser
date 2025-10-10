#ifndef SET_H
#define SET_H

#include <string>
#include "card.h"
#include <map>
class Set
{
public:
    Set(std::string key,std::string name,std::string path);
    Set();
    std::string getKey();
    std::string getName();
    std::string getPath();
    void setKey(std::string key);
    void setName(std::string name);
    std::map<std::string,Card*>* getCards();
    Card* getCard(std::string code);
    void setPath(std::string path);
    bool containsCard(std::string code);
private:
    std::string key;
    std::string name;
    std::string path;
    std::map<std::string,Card*> cards;
};

#endif // SET_H
