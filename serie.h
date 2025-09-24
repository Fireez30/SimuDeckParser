#ifndef SERIE_H
#define SERIE_H

#include<vector>
#include "set.h"
#include<string>
class Serie
{
public:
    Serie(std::string name,std::string path);
    std::string getName();
    std::string getPath();
    std::vector<Set> &getAllSets();
    Set getSetAtIndex(int index);
    void setSets(std::vector<Set> sets);
    void AddSet(std::string name,std::string key, std::string path);
private:
    std::string name; // This is used as identifier for the serie
    std::string path; // path to serie
    std::vector<Set> sets;
};

#endif // SERIE_H
