#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QListWidgetItem>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>


class cardlist;
class MainMenu;
class deckvisualization;
class importdeck;
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
    void SwitchToCardList();
    void SwitchToDeckEditor();
    void SwitchToMainMenu();
    void SwitchToImportDeck();
private:
    Ui::MainWindow *ui;
    MainMenu* mm;
    cardlist *cl;
    deckvisualization* dv;
    importdeck* id;
};
#endif // MAINWINDOW_H
