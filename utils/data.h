#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
#include "algorithms.h"
#include "set.h"
#include <QFile>
#include "serie.h"
#include "deck.h"

void ParseCards(Set& set);
void ParseSets(Serie& serie,std::string card_path);
std::vector<Serie> ParseSeries(std::string cards_path);
void ParseCommonEffects(std::string simulator_path);
void ParseDecks(std::map<std::string,Deck> &decks);
void LoadDeck(std::map<std::string,Deck> &decks, std::string target_deck_name,std::vector<Serie> &series);
#endif // DATA_H
