#include "importdeck.h"
#include "ui_importdeck.h"
#include <QUiLoader>
#include <QFile>
#include "algorithms.h"
#include "mainwindow.h"
importdeck::importdeck(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::importdeck)
{
    ui->setupUi(this);
    this->ui->EncoreDeckLinkField->clear();
    connect(this->ui->ImportButton, SIGNAL (clicked(bool)),this, SLOT (ImportLink()));
    //connect(this->ui->startImport, SIGNAL (clicked(bool)),this, SLOT (ImportLink()));
    connect(this->ui->closeImportPanelButton, SIGNAL(clicked(bool)),this,SLOT(SwitchToMainMenu()));
}

importdeck::~importdeck()
{
    delete ui;
}


void importdeck::ImportLink(){
    /*std::string link = this->ui->EncoreDeckLinkField->text().toStdString();
    std::vector<std::string> splitted_link  = split(link,'/');
    if (splitted_link.size() > 4){
        ParseDeckById(splitted_link.back(),this->decks,this->series);
    }

    this->HideImportPanel();*/
}



void importdeck::SwitchToMainMenu(){
    MainWindow* mw = getMainWindow();
    mw->SwitchToMainMenu();
}


