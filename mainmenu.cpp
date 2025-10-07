#include <QSignalMapper>
#include <QScrollBar>
#include<QLineEdit>
#include "algorithms.h"
#include "io.h"
#include <QCheckBox>
#include "hoverlabel.h"
#include "mainmenu.h"
#include "ui_mainmenu.h"
#include <QUiLoader>
#include <QComboBox>
#include <QFile>
#include <QLabel>
#include <QScrollArea>
#include <QMessageBox>
#include <filesystem>
#include "dataloader.h"
#include "mainmenu.h"
namespace fs = std::filesystem;


MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    this->ui->simulatorWidget->setVisible(true);
    this->ui->simulatorWidget->setAttribute(Qt::WA_TransparentForMouseEvents,false);
    this->ui->MenusWidget->setVisible(false);
    this->ui->MenusWidget->setAttribute(Qt::WA_TransparentForMouseEvents,true);

    std::string previous_path = GetSetting("simulator_data_path");
    if (previous_path != ""){
        if (!endsWith(previous_path,std::to_string(separator()))){
            previous_path += separator();
        }
        this->LoadSimulator(GetSetting("simulator_data_path"));
    }
    connect(this->ui->loadSimulatorButton, SIGNAL (clicked(bool)), this, SLOT (LoadButtonClicked()));
    connect(this->ui->MainMenuCardListButton, SIGNAL (clicked(bool)),this, SLOT (SwitchToCardList()));
    connect(this->ui->MainMenuDeckEditorButton, SIGNAL (clicked(bool)),this, SLOT (SwitchToDeckEditor()));
    connect(this->ui->MainMenuQuitButton, SIGNAL (clicked(bool)),this, SLOT (OnExit()));
    connect(this->ui->unloadButton, SIGNAL (clicked(bool)),this, SLOT (OnUnloadSimulator()));
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::SwitchToMainMenu(){
    MainWindow* mw = getMainWindow();
    mw->SwitchToMainMenu();
}

void MainMenu::SwitchToCardList(){
    MainWindow* mw = getMainWindow();
    mw->SwitchToCardList();
}

void MainMenu::SwitchToDeckEditor(){
    MainWindow* mw = getMainWindow();
    mw->SwitchToDeckEditor();
}

void MainMenu::OnExit(){
    QApplication::quit();
}

void MainMenu::SwitchToImportDeck(){
    MainWindow* mw = getMainWindow();
    mw->SwitchToImportDeck();
}


void MainMenu::OnUnloadSimulator(){
    DeleteSetting("simulator_data_path");
    this->ui->simulatorWidget->setVisible(true);
    this->ui->simulatorWidget->setAttribute(Qt::WA_TransparentForMouseEvents,false);
    this->ui->MenusWidget->setVisible(false);
    this->ui->MenusWidget->setAttribute(Qt::WA_TransparentForMouseEvents,true);
}


void MainMenu::LoadSimulator(std::string simulator_path){
    if (fs::exists(simulator_path)){
        DataLoader::GetInstance()->ParseCommonEffects(simulator_path);
        // internal subfolders
        std::string cards_folder = simulator_path+"StreamingAssets"+separator()+"Cards";
        // parsing all series
        DataLoader::GetInstance()->ParseSeries(cards_folder);

        this->ui->simulatorWidget->setVisible(false);
        this->ui->simulatorWidget->setAttribute(Qt::WA_TransparentForMouseEvents,true);
        this->ui->MenusWidget->setVisible(true);
        this->ui->MenusWidget->setAttribute(Qt::WA_TransparentForMouseEvents,false);
    }
    else
    {
        QMessageBox::information(this, "Path chosen",
                                 "Simulator path is incorrect, verify and try again");
    }
}
void MainMenu::LoadButtonClicked(){
    //"/home/ben/Games/Weiss Schwarz 0.6.3.2 Linux/Weiss Schwarz 0.6.3.2 Linux_Data/"
    std::string simulator_path { this->ui->simulatorPathBox->text().toStdString() };
    if (!endsWith(simulator_path,std::to_string(separator()))){
        simulator_path += separator();
    }
    if (fs::exists(simulator_path)){
        AddOrUpdateSetting("simulator_data_path",simulator_path);
    }
    this->LoadSimulator(simulator_path);

}
