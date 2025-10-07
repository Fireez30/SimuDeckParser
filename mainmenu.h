#ifndef MAINMENU_H
#define MAINMENU_H
#include <QWidget>
namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();
    void LoadSimulator(std::string simulator_path);

private:
    Ui::MainMenu *ui;

private slots:
    void LoadButtonClicked();
    void OnUnloadSimulator();
    void OnExit();
    void SwitchToCardList();
    void SwitchToDeckEditor();
    void SwitchToMainMenu();
    void SwitchToImportDeck();
};

#endif // MAINMENU_H
