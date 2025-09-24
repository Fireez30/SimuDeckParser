#include "card.h"
#include "utils.h"
#include <iostream>
Card::Card() {
    this->triggers = {};
    this->traits = {};
}

Card::Card(std::string key,CardType type,std::string name,std::string path,Color color,uint level,uint cost, int power,Trigger trigger1, Trigger trigger2,uint soul_count,std::string code, std::string text,std::string trait1,std::string trait2,std::string trait3){
    this->key = key;
    this->type = type;
    this->name = name;
    this->image_path=path;
    this->color = color;
    this->level = level;
    this->cost = cost;
    this->power = power;
    this->triggers = {};
    if (trigger1 != Trigger::NONE){
        this->triggers.push_back(trigger1);
    }
    if (trigger2 != Trigger::NONE){
        this->triggers.push_back(trigger2);
    }
    this->soul_count = soul_count;
    this->simulator_code = code;
    this->text = text;
    this->traits = {};
    if (trait1 != ""){
        this->traits.push_back(trait1);
    }
    if (trait2 != ""){
        this->traits.push_back(trait2);
    }

    if (trait3 != ""){
        this->traits.push_back(trait3);
    }
}

void Card::print(){
    std::string card_text = this->getWholeCardText();

    std::cout << " ------------------ " << std::endl;
    std::cout << card_text << std::endl;
}

std::string Card::getWholeCardText(){
    std::string whole_text;
    whole_text += " Card : " + this->getKey() + " (" + GetCardTypeString(this->type) + ") \n";
    whole_text += "    name : " + this->getName() + "\n";
    whole_text += "    level "+ std::to_string(this->getLevel()) + " / cost " + std::to_string(this->getCost()) + "\n";
    whole_text += "    power : " + std::to_string(this->getPower()) + "\n";
    whole_text += "    color : " + GetColorString(this->getColor()) + "\n";
    if (this->triggers.size() > 0){
        whole_text += "    triggers : ";
        for (Trigger t : this->triggers){
            whole_text += GetTriggerString(t) + " ";
        }
        whole_text += "\n";
    }
    whole_text += "    souls : " + std::to_string(this->getSoulCount()) + "\n";
    if (this->triggers.size() > 0){
        for (int i {0}; i < this->getTraits().size() ; ++i){
            whole_text += "trait " + std::to_string(i) + " : " + this->getTraits()[i] + "\n";
        }
    }
    whole_text += "    text : " + this->getText() + "\n";
    whole_text +="    code : " + this->getSimulatorCode() + "\n";
    whole_text += "    image : " + this->getImagePath() + "\n";
    return whole_text;
}

CardType Card::getCardType(){
    return this->type;
}
std::string Card::getKey(){
    return this->key;
}
std::string Card::getName(){
    return this->name;
}
std::string Card::getImagePath(){
    return this->image_path;
}
Color Card::getColor(){
    return this->color;
}
uint Card::getLevel(){
    return this->level;
}
uint Card::getCost(){
    return this->cost;
}
int Card::getPower(){
    return this->power;
}
std::vector<Trigger> Card::getTriggers(){
    return this->triggers;
}
uint Card::getSoulCount(){
    return this->soul_count;
}
std::vector<std::string> Card::getTraits(){
    return this->traits;
}
std::string Card::getSimulatorCode(){
    return this->simulator_code;
}
std::string Card::getText(){
    return this->text;
}
