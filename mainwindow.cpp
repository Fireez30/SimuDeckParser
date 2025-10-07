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
    this->mm = new MainMenu(this);
    this->cl = new cardlist(this);
    this->dv = new deckvisualization(this);
    this->id = new importdeck(this);

    this->ui->stackedWidget->addWidget(mm);
    this->ui->stackedWidget->addWidget(cl);
    this->ui->stackedWidget->addWidget(dv);
    this->ui->stackedWidget->addWidget(id);
    this->SwitchToMainMenu();

    //binding signals
    //
}


void MainWindow::SwitchToMainMenu(){
    this->ui->stackedWidget->setCurrentWidget(this->mm);
}

void MainWindow::SwitchToCardList(){
    this->ui->stackedWidget->setCurrentWidget(this->cl);
}

void MainWindow::SwitchToDeckEditor(){
    this->ui->stackedWidget->setCurrentWidget(this->dv);
}

void MainWindow::SwitchToImportDeck(){
    this->ui->stackedWidget->setCurrentWidget(this->id);
}


MainWindow::~MainWindow()
{
    delete ui;
}


