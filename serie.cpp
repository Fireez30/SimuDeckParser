#include "serie.h"
#include <string>

Serie::Serie(std::string name,std::string serie_path) {
    this->name = name;
    this->path = serie_path;
    this->sets = {};
}

std::string Serie::getPath(){
    return this->path;
}
std::string Serie::getName(){
    return this->name;
}

std::vector<Set> Serie::getAllSets(){
    return this->sets;
}

Set Serie::getSetAtIndex(int index){
    if (index < 0 || index < this->sets.size()){
        return Set("","",""); // This sucks, should use something else
    }
    return this->sets.at(index);
}

void Serie::setSets(std::vector<Set> sets){
    this->sets = sets;
}
