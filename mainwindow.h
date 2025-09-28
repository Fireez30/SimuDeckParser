#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "serie.h"
#include "algorithms.h"
#include <QMainWindow>
#include <QListWidgetItem>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include "deck.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr,int grid_width=3,int grid_height=4);
    ~MainWindow();
    std::vector<Serie> getAllSeries();
    Serie getSerieByName(std::string name);
    void UpdateUi();
    int getGridCount();
    int current_cards_index;
    void FillFiltersUsingSet();
    void FillCards();
    void UnloadData(bool unload_set_wiget=true);
    void ApplyFilter();
    void LoadSimulator(std::string simulator_path);
    void SortFilteredCards();
    void RemoveFilter(std::string filter);
    void AddFilter(std::string filter);
    void RemoveOrder(std::string order);
    void AddOrder(std::string order);
    void RedisplayAfterFilter();
    void DisplayFilteredCards();
    std::vector<QLabel*> currentCardsImages;
    void DestroyDisplayedCards();
    void ClearCardsWidget();
    void DisplayPickedDeck();
    void ClearDeckWidget();
private:
    Ui::MainWindow *ui;
    Serie* choosen_serie;
    std::vector<Set*> choosen_sets;
    std::vector<Serie> series;
    std::map<std::string,Deck> decks;
    bool display_main_menu;
    bool display_card_list;
    bool display_deck_editor;
    bool display_load_series;
    bool display_pick_set;
    bool display_cards;
    std::string picked_deck;
    std::vector<int> available_level_filters;
    std::vector<std::string> available_trait_filters;
    std::vector<int> available_cost_filters;
    std::vector<Color> available_color_filters;
    std::vector<int> current_level_filters;
    std::vector<int> current_cost_filters;
    std::vector<Color> current_color_filters;
    std::vector<std::string> current_trait_filters;
    std::vector<CardType> available_type_filters;
    std::vector<CardType> current_type_filters;
    std::vector<QListWidgetItem*> cards_items;
    int grid_width;
    std::vector<Orders> current_orders;
    int grid_height;
    std::vector<Card> current_cards_to_display;
    std::vector<Card> all_cards_available;
private slots:
    void LoadButtonClicked();
    void OnSeriePick();
    void OnSerieCardsPick();
    void OnSetPick();
    void OnExit();
    void OnUnloadSimulator();
    void TestFiltersAndSorts();
    void AddLevelFilter(bool active,int level);
    void AddTraitFilter(bool active,std::string trait);
    void AddCostFilter(bool active, int cost);
    void AddColorFilter(bool active,Color color);
    void AddTypeFilter(bool active, CardType type);
    void ApplyFilters();
    void SwitchToMainMenu();
    void SwitchToCardList();
    void SwitchToDeckEditor();
    void PickDeck();
    void ShowCardToSidePanel(Card c);
};
#endif // MAINWINDOW_H
