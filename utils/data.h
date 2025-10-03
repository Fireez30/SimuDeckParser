#ifndef DATA_H
#define DATA_H


#include <string>
#include <vector>
#include "algorithms.h"
#include "set.h"
#include <QFile>
#include "serie.h"
#include "deck.h"
#include "json.hpp"
using json = nlohmann::json;


void ParseCards(Set& set);
void ParseSets(Serie& serie,std::string card_path);
std::vector<Serie> ParseSeries(std::string cards_path);
void ParseCommonEffects(std::string simulator_path);
void ParseDecks(std::map<std::string,Deck> &decks);
void LoadDeck(std::map<std::string,Deck> &decks, std::string target_deck_name,std::vector<Serie> &series);
void LoadDeckFromList(std::map<std::string,Deck> &decks,std::vector<Serie> &series,std::vector<std::string> card_list,std::string deck_name);
void ParseDeckFromJson(json deck_json,std::map<std::string,Deck> &decks,std::vector<Serie> &series,bool print=false);
json SendRequest(std::string url);
void ParseDeckById(std::string deck_code,std::map<std::string,Deck> &decks,std::vector<Serie> &series);
std::vector<std::string> TransformToExistingCardKey(std::vector<Serie> &series,const std::vector<std::string> card_keys);
#endif // DATA_H
