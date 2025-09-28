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

std::map<std::string,Card> &Set::getCards(){
    return this->cards;
}

bool Set::containsCard(std::string code){
    std::map<std::string,Card>::iterator it;
    it = this->cards.find(code);
    return (it != this->cards.end());
}

Card* Set::getCard(std::string code){
    std::map<std::string,Card>::iterator it;
    it = this->cards.find(code);
    if (it != this->cards.end()){
        return &((*it).second);
    }
    return nullptr; // Maybe there's a smarter thing to do
}


