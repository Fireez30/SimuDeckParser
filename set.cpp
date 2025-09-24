#include "set.h"
Set::Set(std::string key,std::string name,std::string path){
    this->key = key;
    this->name = name;
    this->path = path;
    this->cards = {};
}

Set::Set(){
    this->cards = {};
}

std::string Set::getKey(){
    return this->key;
}
std::string Set::getName(){
    return this->name;
}
std::string Set::getPath(){
    return this->path;
}

void Set::setKey(std::string key){
    this->key = key;
}
void Set::setName(std::string name){
    this->name = name;
}
void Set::setPath(std::string path){
    this->path = path;
}

std::vector<Card> &Set::getCards(){
    return this->cards;
}

Card Set::getCardAt(int index){
    if (index < 0 || index < this->cards.size()){
        return Card(); // This sucks, should use something else
        }
    return this->cards.at(index);
}


