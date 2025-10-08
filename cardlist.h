#ifndef CARDLIST_H
#define CARDLIST_H
#include "card.h"
#include <QListWidgetItem>
#include <QWidget>
#include <QLabel>
#include "set.h"
namespace Ui {
class cardlist;
}

class cardlist : public QWidget
{
    Q_OBJECT

public:
    explicit cardlist(QWidget *parent = nullptr);
    ~cardlist();
    void ClearCardsWidget(bool clear_filters=true);
    std::vector<QLabel*> currentCardsImages;
    void DestroyDisplayedCards();
    void RedisplayAfterFilter();
    void DisplayFilteredCards();
    void RemoveFilter(std::string filter);
    void AddFilter(std::string filter);
    void RemoveOrder(std::string order);
    void AddOrder(std::string order);
    void FillFiltersUsingSet();
    void FillCards();
    void UnloadData(bool unload_set_wiget=true);
    void ApplyFilter();
    void UpdateUi();

private:
    Serie* choosen_serie;
    std::vector<Set*> choosen_sets;
    bool picked_serie;
    bool picked_sets;
    Ui::cardlist *ui;
    std::vector<Card> all_cards_available;
    std::vector<CardType> available_type_filters;
    std::vector<int> available_level_filters;
    std::vector<std::string> available_trait_filters;
    std::vector<int> available_cost_filters;
    std::vector<Color> available_color_filters;
    std::vector<int> current_level_filters;
    std::vector<int> current_cost_filters;
    std::vector<Color> current_color_filters;
    std::vector<std::string> current_trait_filters;
    std::vector<CardType> current_type_filters;
    std::vector<QListWidgetItem*> cards_items;
    std::vector<Orders> current_orders;

    std::vector<Card> current_cards_to_display;

private slots:
    void OnSeriePick();
    void OnSerieCardsPick();
    void OnSetPick();
    void AddLevelFilter(bool active,int level);
    void AddTraitFilter(bool active,QStringList checked_traits);
    void AddCostFilter(bool active, int cost);
    void AddColorFilter(bool active,Color color);
    void AddTypeFilter(bool active, CardType type);
    void ApplyFilters();
    void SwitchToMainMenu();
    void ResetTraitComboBox();
};

#endif // CARDLIST_H
