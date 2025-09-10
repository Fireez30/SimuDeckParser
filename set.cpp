#include "set.h"

Set::Set(std::string key,std::string name,std::string path){
    this->key = key;
    this->name = name;
    this->path = path;
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
