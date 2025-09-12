#ifndef SET_H
#define SET_H

#include <string>
#include "card.h"
#include <vector>
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
    std::vector<Card> &getCards();
    void setPath(std::string path);
private:
    std::string key;
    std::string name;
    std::string path;
    std::vector<Card> cards;
};

#endif // SET_H
