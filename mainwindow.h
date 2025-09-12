#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "serie.h"
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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::vector<Serie> getAllSeries();
    Serie getSerieByName(std::string name);
    void UpdateUi();

private:
    Ui::MainWindow *ui;
    Serie* choosen_serie;
    Set* choosen_set;
    std::vector<Serie> series;
    bool display_load_series;
    bool display_pick_set;
    bool display_cards;
    QLineEdit *m_simu_path_field;
    QComboBox *m_series_dropdown;
    QPushButton *m_button_loadseries;
    QPushButton *m_series_pick_button;
    QPushButton *m_set_pick_button;
    QComboBox *m_set_dropdown;
private slots:
    void LoadButtonClicked();
    void OnSeriePick();
    void OnSetPick();
};
#endif // MAINWINDOW_H
