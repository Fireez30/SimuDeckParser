#include <string>
#include "algorithms.h"
#include <QFile>
#include <algorithm>
namespace fs = std::filesystem;

std::string trim(const std::string &s)
{
    auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c){return std::isspace(c);});
    auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c){return std::isspace(c);}).base();
    return (wsback<=wsfront ? std::string() : std::string(wsfront, wsback));
}

std::string removeTrailingAlphas(const std::string& input) {
    int end = input.length() - 1;
    while (end >= 0 && std::isalpha(static_cast<unsigned char>(input[end]))) {
        --end;
    }
    return input.substr(0, end + 1);
}

std::string GetColorString(Color c){
    switch (c){
    case Color::YELLOW:
        return "Yellow";
    case Color::BLUE:
        return "Blue";
    case Color::RED:
        return "Red";
    case Color::GREEN:
        return "Green";
    case Color::PURPLE:
        return "Purple";
    case Color::ALL:
        return "All";
    default:
        return "None";
    }
}

void replace_in_string(std::string &str, const std::string &a, const std::string &b) {
    size_t pos = 0;
    while ((pos = str.find(a, pos)) != std::string::npos) {
        str.replace(pos, a.length(), b);
        pos += b.length(); // On avance de la longueur de b pour éviter les boucles infinies
    }
}

std::string GetTriggerString(Trigger t){
    switch (t){
        case Trigger::BAG:
            return "Goldbag";
        case Trigger::BAR:
            return "Goldbar";
        case Trigger::BOOK:
            return "Book";
        case Trigger::BURN:
            return "Burn";
        case Trigger::CHOICE:
            return "Choice";
        case Trigger::PANT:
            return "Pant";
        case Trigger::SALVAGE:
            return "Salvage";
        case Trigger::SOUL:
            return "Soul";
        case Trigger::STANDBY:
            return "Standby";
        case Trigger::WIND:
            return "Wind";
        default:
            return "None";
    }
}


std::string GetCardTypeString(CardType t){
    switch (t){
    case CardType::CHARACTER:
        return "Character";
    case CardType::EVENT:
        return "Event";
    default:
        return "Climax";
    }
}




std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0, end = s.find(delimiter);
    while (end != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + 1;
        end = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start));
    return tokens;
}


bool findSubstringIgnoreCase(const std::string& X, const std::string& Y) {
    if (X.empty()) return true; // empty string is always found

    std::string lowerX, lowerY;
    lowerX.reserve(X.size());
    lowerY.reserve(Y.size());

    // Convert X to lowercase
    std::transform(X.begin(), X.end(), std::back_inserter(lowerX),
                   [](unsigned char c) { return std::tolower(c); });

    // Convert Y to lowercase
    std::transform(Y.begin(), Y.end(), std::back_inserter(lowerY),
                   [](unsigned char c) { return std::tolower(c); });

    // Search for lowerX in lowerY
    return lowerY.find(lowerX) != std::string::npos;
}

bool endsWith(const std::string& s, const std::string& suffix)
{
    return s.rfind(suffix) == (s.size() - suffix.size());
}

//Determine pass separator using OS
char separator()
{
#if defined _WIN32 || defined __CYGWIN__
    return '\\';
#else
    return '/';
#endif
}





