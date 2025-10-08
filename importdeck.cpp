#include "importdeck.h"
#include "ui_importdeck.h"
#include <QUiLoader>
#include <QFile>
#include "algorithms.h"
#include "mainwindow.h"
#include "dataloader.h"
#include <iostream>
importdeck::importdeck(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::importdeck)
{
    ui->setupUi(this);
    //this->ui->ImportTextLayout->setvisible(false);
    this->ui->simulatorListWidget->setVisible(false);
    this->ui->horizontalWidget->setVisible(false);
    connect(this->ui->ImportButton, SIGNAL (clicked(bool)),this, SLOT (ImportLink()));
    connect(this->ui->closeImportPanelButton, SIGNAL(clicked(bool)),this,SLOT(SwitchToMainMenu()));
    connect(this->ui->startImport,SIGNAL (clicked(bool)),this, SLOT (AddToSimu()));
}

importdeck::~importdeck()
{
    delete ui;
}


void importdeck::ImportLink(){
    this->ui->horizontalWidget->setVisible(true);
    std::vector<std::string> deck_data = {};
    std::string link = this->ui->EncoreDeckLinkField->text().toStdString();
    std::vector<std::string> splitted_link  = split(link,'/');
    if (splitted_link.size() > 4){
        deck_data = DataLoader::GetInstance()->ParseDeckById(splitted_link.back());
    }

    if (deck_data.size() > 2){
        this->deck_name = deck_data.back();
        deck_data.pop_back();
        this->deck_date = deck_data.back();
        deck_data.pop_back();
        this->deck_init_card_list = deck_data;
    }
    this->ui->deckDateLabel->setText(QString().fromStdString("Creation date : "+this->deck_date));
    this->ui->deckNameLabel->setText(QString().fromStdString(this->deck_name));
    std::string base_deck_to_display = "";
    for (std::string card : this->deck_init_card_list){
        base_deck_to_display += card + "\n";
    }
    this->ui->encoreDecksList->setText(QString().fromStdString(base_deck_to_display));
    this->deck_final_card_list = DataLoader::GetInstance()->AdaptDeckList(deck_data);
    this->ui->simulatorListWidget->setVisible(true);
    std::string base_deck_to_display_new = "";
    for (std::string card : this->deck_final_card_list){
        base_deck_to_display_new += card + "\n";
    }
    this->ui->encoreDecksList_2->setText(QString().fromStdString(base_deck_to_display_new));
}

void importdeck::AddToSimu(){
    std::vector<std::string> final_list = {};
    std::vector<std::string> decksplit = split(this->ui->encoreDecksList_2->toPlainText().toStdString(),'\n');
    if (decksplit.size() > 0){
        for (std::string card : decksplit){
            if (trim(card) != ""){
                final_list.push_back(trim(card));
            }
        }
    }

    if (final_list.size() == 50){
        bool added = DataLoader::GetInstance()->AddDeckToSimulator(this->deck_name,this->deck_date,final_list);
        if (added){
            DataLoader::GetInstance()->getDecks()->insert_or_assign(deck_name, new Deck(deck_name,deck_date));
            DataLoader::GetInstance()->LoadDeckFromList(final_list,deck_name);
        }
        else {
            std::cout << "ERROR ADDING TO SIMULATOR" << std::endl;
            // transfer backuped file to prefs file to reset changes
    }

    }
}


void importdeck::SwitchToMainMenu(){
    MainWindow* mw = getMainWindow();
    mw->SwitchToMainMenu();
}


