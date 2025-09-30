#include "set.h"
#include<iostream>
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

std::string removeTrailingAlphas(const std::string& input) {
    int end = input.length() - 1;
    while (end >= 0 && std::isalpha(static_cast<unsigned char>(input[end]))) {
        --end;
    }
    return input.substr(0, end + 1);
}

bool Set::containsCard(std::string code){
    std::map<std::string,Card>::iterator it;
    it = this->cards.find(code);
    // BUG HERE ! IF CARD CODE ENDS WITH A LETTER (IF THERE IS 4 or 5 CHARACTER), AND IT NOT FOUND, VERIFY IF REMOVING LAST CHARACTER
    if (it == this->cards.end()){
        std::string new_code = removeTrailingAlphas(code);
        //std::cout << "Retrying from " << code << " to " << new_code << std::endl;
        it = this->cards.find(new_code);
    }
    return (it != this->cards.end());
}

Card* Set::getCard(std::string code){
    std::map<std::string,Card>::iterator it;
    it = this->cards.find(code);
    if (it != this->cards.end()){
        return &((*it).second);
    }
    else {
        if (it == this->cards.end()){
            std::string new_code = removeTrailingAlphas(code);
            if (code != new_code){
                //std::cout << "Retrying from " << code << " to " << new_code << std::endl;
                it = this->cards.find(new_code);
                if (it != this->cards.end()){
                    return &((*it).second);
                }
            }
        }
    }
    // BUG HERE ! IF CARD CODE ENDS WITH A LETTER (IF THERE IS 4 CHARACTER) AND IT NOT FOUND, VERIFY IF REMOVING LAST CHARACTER
    return nullptr; // Maybe there's a smarter thing to do
}


