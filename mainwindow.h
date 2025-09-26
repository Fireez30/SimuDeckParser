#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "serie.h"
#include "algorithms.h"
#include <QMainWindow>
#include <QListWidgetItem>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
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
    void UnloadData(bool unload_set_wiget=true);
    void FillCardsUsingFilters();
    void LoadSimulator(std::string simulator_path);
    void SortFilteredCards();
    void RemoveFilter(std::string filter);
    void AddFilter(std::string filter);
    void RemoveOrder(std::string order);
    void AddOrder(std::string order);
    void DisplayFilteredCards();
    std::vector<QLabel*> currentCardsImages;
    void DestroyDisplayedCards();
    void ClearCardsWidget();
private:
    Ui::MainWindow *ui;
    Serie* choosen_serie;
    std::vector<Set*> choosen_sets;
    std::vector<Serie> series;
    bool display_load_series;
    bool display_pick_set;
    bool display_cards;
    std::vector<int> available_level_filters;
    std::vector<int> available_cost_filters;
    std::vector<Color> available_color_filters;
    std::vector<int> current_level_filters;
    std::vector<int> current_cost_filters;
    std::vector<Color> current_color_filters;
    std::vector<CardType> available_type_filters;
    std::vector<CardType> current_type_filters;
    std::vector<QListWidgetItem*> cards_items;
    int grid_width;
    std::vector<Orders> current_orders;
    int grid_height;
    std::vector<Card> current_cards_to_display;
private slots:
    void LoadButtonClicked();
    void OnSeriePick();
    void OnSerieCardsPick();
    void OnSetPick();
    void OnExit();
    void OnUnloadSimulator();
    void TestFiltersAndSorts();
    void AddLevelFilter(bool active,int level);
    void AddColorFilter(bool active,Color color);
};
#endif // MAINWINDOW_H
