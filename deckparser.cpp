#include "deckparser.h"

DeckParser::DeckParser(std::string_view simulator_path) {
    this->simulator_path = std::string{simulator_path};
}
