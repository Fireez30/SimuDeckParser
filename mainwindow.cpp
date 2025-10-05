#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSignalMapper>
#include <QScrollBar>
#include<QLineEdit>
#include "algorithms.h"
#include "io.h"
#include <QCheckBox>
#include "hoverlabel.h"
#include <QUiLoader>
#include <QComboBox>
#include <QFile>
#include <QLabel>
#include <QScrollArea>
#include <QMessageBox>
#include <filesystem>
#include "dataloader.h"
namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //binding signals
    connect(this->ui->loadSimulatorButton, SIGNAL (clicked(bool)), this, SLOT (LoadButtonClicked()));
    connect(this->ui->actionExit, SIGNAL (triggered()), this, SLOT (OnExit()));
    connect(this->ui->actionUnload_simulator, SIGNAL (triggered()), this,SLOT (OnUnloadSimulator()));
    connect(this->ui->MainMenuCardListButton, SIGNAL (clicked(bool)),this, SLOT (SwitchToCardList()));
    connect(this->ui->MainMenuDeckEditorButton, SIGNAL (clicked(bool)),this, SLOT (SwitchToDeckEditor()));
    connect(this->ui->MainMenuQuitButton, SIGNAL (clicked(bool)),this, SLOT (OnExit()));
    connect(this->ui->unloadButton, SIGNAL (clicked(bool)),this, SLOT (OnUnloadSimulator()));

    this->ui->simulatorWidget->setVisible(true);
    this->ui->simulatorWidget->setAttribute(Qt::WA_TransparentForMouseEvents,false);
    this->ui->MenusWidget->setVisible(false);
    this->ui->MenusWidget->setAttribute(Qt::WA_TransparentForMouseEvents,true);

    std::string previous_path = GetSetting("simulator_data_path");
    if (previous_path != ""){
        if (!endsWith(previous_path,std::to_string(separator()))){
            previous_path += separator();
        }
        LoadSimulator(GetSetting("simulator_data_path"));
    }
    //
}


void MainWindow::switchToUi(const QString &uiFilePath)
{
    QUiLoader loader;
    QFile file(uiFilePath);
    if (file.open(QFile::ReadOnly)) {
        QWidget *newWidget = loader.load(&file, this);
        file.close();
        setCentralWidget(newWidget);
    }
}




void MainWindow::SwitchToCardList(){
    this->switchToUi(QString(":/forms/cardlist.ui"));
}

void MainWindow::SwitchToDeckEditor(){
    this->switchToUi(QString(":/forms/deckvisualization.ui"));
}



MainWindow::~MainWindow()
{
    delete ui;
}

void RemoveFilter(std::string filter){
    // parser to determine what filter will be used


}
void AddFilter(std::string filter);
void RemoveOrder(std::string order);
void AddOrder(std::string order);



void MainWindow::OnExit(){
    QApplication::quit();
}

void MainWindow::OnUnloadSimulator(){
    DeleteSetting("simulator_data_path");
    this->ui->simulatorWidget->setVisible(true);
    this->ui->simulatorWidget->setAttribute(Qt::WA_TransparentForMouseEvents,false);
    this->ui->MenusWidget->setVisible(false);
    this->ui->MenusWidget->setAttribute(Qt::WA_TransparentForMouseEvents,true);
}


void MainWindow::LoadSimulator(std::string simulator_path){
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
void MainWindow::LoadButtonClicked(){
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
