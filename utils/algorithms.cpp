#include <string>
#include "algorithms.h"
#include "set.h"
#include "serie.h"
#include <QFile>
#include <QWidget>
#include <algorithm>
#include <QLayout>
#include "../mainwindow.h"
#include <qapplication.h>
namespace fs = std::filesystem;


MainWindow* getMainWindow()
{
    foreach (QWidget *w, qApp->topLevelWidgets())
    if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        return mainWin;
    return nullptr;
}

std::string base64_encode(const std::string& input) {
    static const char encoding_table[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string output;
    int val = 0;
    int valb = -6;

    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;

        while (valb >= 0) {
            output.push_back(encoding_table[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }

    if (valb > -6) {
        output.push_back(encoding_table[((val << 8) >> (valb + 8)) & 0x3F]);
    }

    while (output.size() % 4) {
        output.push_back('=');
    }

    return output;
}

std::string base64_decode(const std::string &in) {

    std::string out;

    std::vector<int> T(256,-1);
    for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val=0, valb=-8;
    for (uchar c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val>>valb)&0xFF));
            valb -= 8;
        }
    }
    return out;
}

QColor GetQColorFromCardColor(Color c){
    if (c == Color::GREEN){
        return QColorConstants::Svg::green;
    }

    if (c == Color::BLUE){
        return QColorConstants::Svg::blue;
    }

    if (c == Color::PURPLE){
        return QColorConstants::Svg::purple;
    }

    if (c == Color::RED){
        return QColorConstants::Svg::red;
    }

    if (c == Color::YELLOW)
    {
        return QColorConstants::Svg::yellow;
    }

    return QColorConstants::Svg::white;
}




void SortFilteredCards(std::vector<Card>* cards_to_sort,std::vector<Orders> order){
    std::sort(cards_to_sort->begin(), cards_to_sort->end(), [order](Card a, Card b) { // lambda func
        if (order.size() > 0){
            for (Orders o : order){
                if (o == Orders::TYPE_ASCENDING){
                    if (a.getCardType() != b.getCardType()){
                        return a.getCardType() < b.getCardType();
                    }
                }
                else if (o == Orders::COLOR){
                    if (a.getColor() != b.getColor()){
                        return a.getColor() < b.getColor();
                    }
                }
                else
                {
                    if (a.getCardType() != b.getCardType()){
                        return a.getCardType() < b.getCardType();
                    }
                }

                if (o == Orders::LEVEL_ASCENDING){
                    if (a.getLevel() != b.getLevel()){
                        return a.getLevel() < b.getLevel();
                    }
                }
                else if (o == Orders::LEVEL_DESCENDING){
                    if (a.getLevel() != b.getLevel()){
                        return a.getLevel() > b.getLevel();
                    }
                }

                else if (o == Orders::COST_ASCENDING){
                    if (a.getCost() != b.getCost()){
                        return a.getCost() < b.getCost();
                    }
                }
                else if (o == Orders::COST_DESCENDING){
                    if (a.getCost() != b.getCost()){
                        return a.getCost() > b.getCost();
                    }
                }

                else if (o == Orders::POWER_ASCENDING){
                    if (a.getPower() != b.getPower()){
                        return a.getPower() < b.getPower();
                    }
                }
                else if (o == Orders::POWER_DESCENDING){
                    if (a.getPower() != b.getPower()){
                        return a.getPower() > b.getPower();
                    }
                }
                else if (o == Orders::KEYCODE_ASCENDING){
                    if (a.getKey() != b.getKey()){
                        return a.getKey() < b.getKey();
                    }
                }

            }
        }
        if (a.getLevel() != b.getLevel()){ // default is level > power
            return a.getLevel() < b.getLevel();
        }
        else {
            return a.getPower() > b.getPower(); // default is power but should not happen
        }
    });
}

void clearLayout(QLayout* layout) {
    if (!layout)
        return;

    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();  // Schedule widget for deletion
        }
        if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout);  // Recursively clear nested layouts
            delete childLayout;
        }
        delete item;
    }
}

std::vector<std::string> TransformToExistingCardKey(std::vector<Serie*> &series,const std::vector<std::string> card_keys){ // OPTIMIZE THIS LATER , WANT TO DO SOME TESTS
    std::vector<std::string> final_card_list = {};
    std::vector<std::string> existing_card_codes = {};

    for (Serie* s : series){ // Do not keep this in final release , too slow.
        //std::cout << "is it in serie " << s.getName() << std::endl;
        for (Set set : s->getAllSets()){
            //std::cout << "is it in set " << set.getName() << std::endl;
            std::map<std::string,Card>::iterator it;
            for (it = set.getCards().begin(); it != set.getCards().end(); it++){
                std::string code = (*it).first;
                existing_card_codes.push_back(code);
            }
        }
    }

    // NON FOIL CARD
    //std::string new_code = removeTrailingAlphas(code);
    for (std::string card_key : card_keys){
        bool found = false;
        std::string sub_raw_str;
        std::string sub_str = card_key;
        if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
            final_card_list.push_back(sub_str);
            found = true;
        }

        if (!found){
            sub_str = card_key; // check if to lower
            sub_raw_str = card_key;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }
        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-","-E");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-E","-");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-","-E");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-E","-");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-T","-TE");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-TE","-T");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-T","-TE");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-TE","-T");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-P","-PE");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }
        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-PE","-P");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-P","-PE");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            replace_in_string(sub_str,"-PE","-P");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            replace_in_string(sub_str,"-P","-PE");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            replace_in_string(sub_str,"-PE","-P");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            replace_in_string(sub_str,"-P","-PE");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            replace_in_string(sub_str,"-PE","-P");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }



        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            replace_in_string(sub_str,"-","-E");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            replace_in_string(sub_str,"-E","-");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            replace_in_string(sub_str,"-","-E");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            replace_in_string(sub_str,"-E","-");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }


        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            replace_in_string(sub_str,"-T","-TE");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            replace_in_string(sub_str,"-TE","-T");
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            replace_in_string(sub_str,"-T","-TE");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }

        if (!found){
            sub_str = card_key;
            sub_str = removeTrailingAlphas(sub_str);
            replace_in_string(sub_str,"-TE","-T");
            sub_raw_str = sub_str;
            transform(sub_str.begin(), sub_str.end(), sub_str.begin(),
                      ::tolower);
            if (std::find(existing_card_codes.begin(), existing_card_codes.end(), sub_str) != existing_card_codes.end()) {
                final_card_list.push_back(sub_raw_str);
                found = true;
            }
        }


        if (!found){
            final_card_list.push_back(card_key);
        }


    }

    return final_card_list;
}


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

Color GetStringColor(std::string c){
    if (c == "Yellow"){
        return Color::YELLOW;
    }

    if (c == "Blue"){
        return Color::BLUE;
    }

    if (c == "Red"){
        return Color::RED;
    }

    if (c == "Green"){
        return Color::GREEN;
    }

    if (c == "Purple"){
        return Color::PURPLE;
    }

    if (c == "All"){
        return Color::ALL;
    }
    else return Color::NONE;
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

std::string GetTriggerPath(Trigger t){
    switch (t){
    case Trigger::BAG:
        return "qrc:/images/goldbag.png";
    case Trigger::BAR:
        return "qrc:/images/goldbar.png";
    case Trigger::BOOK:
        return "qrc:/images/book.png";
    case Trigger::BURN:
        return "qrc:/images/burn.png";
    case Trigger::CHOICE:
        return "qrc:/images/switch.png";
    case Trigger::PANT:
        return "qrc:/images/door.png";
    case Trigger::SALVAGE:
        return "qrc:/images/gate.png";
    case Trigger::SOUL:
        return "qrc:/images/soul.png";
    case Trigger::STANDBY:
        return "qrc:/images/standby.png";
    case Trigger::WIND:
        return "qrc:/images/bounce.png";
    default:
        return "qrc:/images/emptycard.jpg";
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

CardType GetStringCardType(std::string t){
    if (t == "Character"){
        return CardType::CHARACTER;
    }

    if (t == "Event"){
        return CardType::EVENT;
    }

    return CardType::CLIMAX;

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





