#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "serie.h"
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>

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
    std::vector<Serie> series;
    bool display_load_series;
    QLineEdit *m_simu_path_field;
    QPushButton *m_button_loadseries;

private slots:
    void LoadButtonClicked();
};
#endif // MAINWINDOW_H
