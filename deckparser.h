#ifndef DECKPARSER_H
#define DECKPARSER_H
#include <string_view>
#include <string>

// This class will manage the loading of sets from the path to Weiss Simulator. Later, it will load decks too
class DeckParser
{

public:
    DeckParser(std::string_view simulator_path);
protected:
    std::string simulator_path; // Path to folder of the simulator
};

#endif // DECKPARSER_H
