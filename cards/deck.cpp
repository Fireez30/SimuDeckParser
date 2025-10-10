#include "deck.h"
#include<string>
#include<vector>
#include<iostream>

Deck::Deck(std::string name,std::string date) {
    this->name = name;
    this->date = date;
    this->card_list = {};
}

void Deck::AddCard(Card* card_ptr){
    this->card_list.push_back(card_ptr);
}

std::string Deck::getName(){
    return this->name;
}
std::string Deck::getDate(){
    return this->date;
}

void Deck::Print(){
    std::cout << this->name << " ( " << this->date << " ) " << std::endl;
    std::cout << this->card_list.size() << std::endl;
    for (Card* card : this->card_list) {
        std::cout << card->getKey() << std::endl;
    }
}

std::vector<Card*>* Deck::getCardList(){
    return &this->card_list;
}
