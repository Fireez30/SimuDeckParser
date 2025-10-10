#ifndef DECK_H
#define DECK_H
#include<string>
#include<vector>
#include"card.h"
class Deck
{
public:
    Deck(std::string name,std::string date);
    void AddCard(Card* card_ptr);
    std::string getName();
    std::string getDate();
    std::vector<Card*>* getCardList();
    void Print();
private:
    std::string name;
    std::string date;
    std::vector<Card*> card_list;
};

#endif // DECK_H
