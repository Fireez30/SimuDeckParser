#ifndef SET_H
#define SET_H

#include <string>

class Set
{
public:
    Set(std::string key,std::string name,std::string path);
    std::string getKey();
    std::string getName();
    std::string getPath();
private:
    std::string key;
    std::string name;
    std::string path;
};

#endif // SET_H
