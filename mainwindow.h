#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "serie.h"
#include "utils.h"
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
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
    void UnloadData();
    void FillCardsUsingFilters();
    void SortFilteredCards();
private:
    Ui::MainWindow *ui;
    Serie* choosen_serie;
    Set* choosen_set;
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
    int grid_width;
    std::vector<Orders> current_orders;
    int grid_height;
    std::vector<Card*> current_cards_to_display;
private slots:
    void LoadButtonClicked();
    void OnSeriePick();
    void OnSetPick();
    void OnExit();
    void OnUnloadSimulator();
};
#endif // MAINWINDOW_H
