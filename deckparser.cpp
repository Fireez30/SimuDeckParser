#include "deckparser.h"

DeckParser::DeckParser(std::string_view streaming_assets_path) {
    this->assets_path = std::string{streaming_assets_path};
}
