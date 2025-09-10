#include "serie.h"
#include <string>

Serie::Serie(std::string name,std::string serie_path) {
    this->name = name;
    this->path = serie_path;
}

std::string Serie::getPath(){
    return this->path;
}
std::string Serie::getName(){
    return this->name;
}
