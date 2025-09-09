#ifndef DECKPARSER_H
#define DECKPARSER_H
#include <string_view>
#include <iostream>
class DeckParser
{

public:
    DeckParser(std::string_view streaming_assets_path);
protected:
    std::string assets_path;
};

#endif // DECKPARSER_H
