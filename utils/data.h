#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
#include "algorithms.h"
#include "set.h"
#include <QFile>
#include "serie.h"

void ParseCards(Set& set);
void ParseSets(Serie& serie,std::string card_path);
std::vector<Serie> ParseSeries(std::string cards_path);
void ParseCommonEffects(std::string simulator_path);
#endif // DATA_H
