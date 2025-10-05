#ifndef DATALOADER_H
#define DATALOADER_H
#include "deck.h"
#include "serie.h"
#include "algorithms.h"
#include "json.hpp"
using json = nlohmann::json;
class DataLoader
{

protected:
    DataLoader();
    static DataLoader* dataloader_;
    std::vector<Serie*> series;
    std::map<std::string,Deck*> decks;
    std::map<std::string,std::string> common_effects;
public:
    DataLoader(DataLoader &other) = delete;
    void operator=(const DataLoader &) = delete;
    static DataLoader *GetInstance();
    std::vector<Serie*>* getAllSeries();
    Serie* getSerieByName(std::string name);
    std::vector<Serie*>* getSeries();
    std::map<std::string,Deck*>* getDecks();
    Deck* getDeck(std::string name);
    Deck* LoadDeck(std::string target_deck_name);
    void ParseCards(Set& set,std::string alternate_cards_folder);
    Deck* LoadDeckFromList(std::vector<std::string> card_list,std::string deck_name);
    std::map<std::string,Deck*>* ParseDecks();
    std::vector<Serie*>* ParseSeries(std::string cards_path);
    void ParseSets(Serie* serie,std::string card_path);
    void ParseCommonEffects(std::string simulator_path);
    void ParseDeckById(std::string deck_code);
    void ParseDeckFromJson(json deck_json,bool print);
    bool AddDeckToSimulator(std::string name,std::string date, std::vector<std::string> card_list,std::string pref_file_path, std::string pref_backup_path);
};

#endif // DATALOADER_H
