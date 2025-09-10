#ifndef SET_H
#define SET_H

#include <string>

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
    void setPath(std::string path);
private:
    std::string key;
    std::string name;
    std::string path;
};

#endif // SET_H
