#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSignalMapper>
#include <QScrollBar>
#include<QLineEdit>
#include <QCheckBox>
#include "hoverlabel.h"
#include <QUiLoader>
#include <QComboBox>
#include <QFile>
#include <QLabel>
#include <QScrollArea>
#include <QMessageBox>
#include "cardlist.h"
#include "deckvisualization.h"
#include "importdeck.h"
#include "mainmenu.h"
namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //binding signals
    MainMenu* mm = new MainMenu(this);
    cardlist *cl = new cardlist(this);
    deckvisualization* dv = new deckvisualization(this);
    importdeck* id = new importdeck(this);
    //this->ui->stackedWidget->removeWidget(this->ui->stackedWidget->widget(0));
    //this->ui->stackedWidget->removeWidget(this->ui->stackedWidget->widget(1));
    this->ui->stackedWidget->addWidget(mm);
    this->ui->stackedWidget->addWidget(cl);
    this->ui->stackedWidget->addWidget(dv);
    this->ui->stackedWidget->addWidget(id);
    this->ui->stackedWidget->setCurrentIndex(0);
    //
}


void MainWindow::SwitchToMainMenu(){
    this->ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::SwitchToCardList(){
    this->ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::SwitchToDeckEditor(){
    this->ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::SwitchToImportDeck(){
    this->ui->stackedWidget->setCurrentIndex(3);
}


MainWindow::~MainWindow()
{
    delete ui;
}


