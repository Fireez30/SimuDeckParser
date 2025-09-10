#include "set.h"
Set::Set(std::string key,std::string name,std::string path){
    this->key = key;
    this->name = name;
    this->path = path;
}

Set::Set(){}

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


