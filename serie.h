#ifndef SERIE_H
#define SERIE_H

#include<string>
class Serie
{
public:
    Serie(std::string name,std::string path);
    std::string getName();
    std::string getPath();
private:
    std::string name; // This is used as identifier for the serie
    std::string path; // path to serie
};

#endif // SERIE_H
