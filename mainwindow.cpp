#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<iostream>
#include <QSignalMapper>
#include <QScrollBar>
#include<QLineEdit>
#include "algorithms.h"
#include "io.h"
#include <QCheckBox>
#include "hoverlabel.h"
#include <QUiLoader>
#include "data.h"
#include <QComboBox>
#include <QFile>
#include <QLabel>
#include <QScrollArea>
#include <QMessageBox>
#include <filesystem>
namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //data
    this->currentCardsImages={};
    this->current_cards_to_display = {};

    this->current_cards_index = 0;
    this->cards_items = {};
    this->available_level_filters = {}; // This will be loaded when laoding a set cards
    this->available_cost_filters = {};
    this->available_color_filters = {};
    this->available_trait_filters = {};
    this->available_type_filters = {};
    this->all_cards_available = {};
    this->UnlockCardPanel();
    this->picked_deck = "";
    this->ui->cardGridWidget_2->setViewMode(QListView::ListMode);
    this->ui->cardGridWidget_2->setResizeMode(QListView::Adjust);
    this->ui->cardGridWidget_2->setIconSize(QSize(280, 391));
    //orders and filters
    this->current_orders = {Orders::KEYCODE_ASCENDING}; // Order with the first, if equals, order with the next, etc ...

    this->current_level_filters = {};
    this->current_cost_filters = {};
    this->current_color_filters = {};
    this->current_type_filters = {};


    this->current_trait_filters = {};

    //display state
    this->display_load_series = true;
    this->display_pick_set = false;
    this->display_cards = false;

    this->display_main_menu = true;
    this->display_card_list = false;
    this->display_deck_editor =false;
    // layouts setup

    this->ui->cardImage->setScaledContents( true );
    this->ui->cardImage->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    this->ui->cardImage->clear();

    this->ui->cardText->clear();
    this->ui->cardText->setWordWrap(true);
    //textLabel->setIndent(224);
    this->ui->cardText->setTextFormat(Qt::RichText);
    this->ui->cardText->setMaximumWidth(1300);
    this->ui->cardText->setAlignment(Qt::AlignLeft);
    this->UpdateUi();
    this->HideImportPanel();
    //binding signals
    connect(this->ui->loadSimulatorButton, SIGNAL (clicked(bool)), this, SLOT (LoadButtonClicked()));
    connect(this->ui->serieLoadButton, SIGNAL (clicked(bool)), this, SLOT (OnSeriePick()));
    connect(this->ui->setLoadButton, SIGNAL (clicked(bool)), this, SLOT (OnSetPick()));
    connect(this->ui->actionExit, SIGNAL (triggered()), this, SLOT (OnExit()));
    connect(this->ui->actionUnload_simulator, SIGNAL (triggered()), this,SLOT (OnUnloadSimulator()));
    //connect(this->ui->testFiltersButton,SIGNAL (clicked(bool)), this, SLOT (TestFiltersAndSorts()));
    connect(this->ui->serieLoadCardsButton,SIGNAL (clicked(bool)),this, SLOT (OnSerieCardsPick()));
    connect(this->ui->ApplyFiltersButton, SIGNAL (clicked(bool)),this, SLOT (ApplyFilters()));
    connect(this->ui->searchButton, SIGNAL (clicked(bool)),this, SLOT (ApplyFilters()));
    connect(this->ui->backButton, SIGNAL (clicked(bool)),this, SLOT (SwitchToMainMenu()));
    connect(this->ui->backButton_2, SIGNAL (clicked(bool)),this, SLOT (SwitchToMainMenu()));
    connect(this->ui->MainMenuCardListButton, SIGNAL (clicked(bool)),this, SLOT (SwitchToCardList()));
    connect(this->ui->MainMenuDeckEditorButton, SIGNAL (clicked(bool)),this, SLOT (SwitchToDeckEditor()));
    connect(this->ui->MainMenuQuitButton, SIGNAL (clicked(bool)),this, SLOT (OnExit()));
    connect(this->ui->pickDeckButton, SIGNAL (clicked(bool)),this, SLOT (PickDeck()));
    connect(this->ui->unloadButton, SIGNAL (clicked(bool)),this, SLOT (OnUnloadSimulator()));
    connect(this->ui->ImportButton, SIGNAL (clicked(bool)),this, SLOT (ImportLink()));
    connect(this->ui->OpenImportWidgetButton, SIGNAL (clicked(bool)),this,SLOT (DisplayImportPanel()));
    connect(this->ui->closeImportPanelButton, SIGNAL (clicked(bool)),this,SLOT (HideImportPanel()));
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
    QUiLoader loader = QUiLoader(this);
    QFile file(uiFilePath);
    if (file.open(QFile::ReadOnly)) {
        QWidget *newWidget = loader.load(&file, this);
        file.close();
        setCentralWidget(newWidget);
    }
}

void MainWindow::DisplayImportPanel(){
    this->ui->backgroundImportWidget->setVisible(true);
    this->ui->EncoreDeckLinkField->clear();
}

void MainWindow::HideImportPanel(){
    this->ui->backgroundImportWidget->setVisible(false);
    this->ui->EncoreDeckLinkField->clear();
}

void MainWindow::ImportLink(){
    std::string link = this->ui->EncoreDeckLinkField->text().toStdString();
    std::vector<std::string> splitted_link  = split(link,'/');
    if (splitted_link.size() > 4){
        ParseDeckById(splitted_link.back(),this->decks,this->series);
    }

    this->HideImportPanel();
}
void MainWindow::SwitchToMainMenu(){
    this->UnloadData(false);

    this->display_main_menu = true;
    this->display_card_list = false;
    this->display_deck_editor =false;

    this->UnlockCardPanel();
    this->UpdateUi();
}

void MainWindow::SwitchToCardList(){
    this->UnloadData(false);
    this->display_main_menu = false;
    this->display_card_list = true;
    this->display_deck_editor =false;
    this->switchToUi(QString(":/forms/cardlist.ui"));
    this->ui = new Ui::cardlist;
    this->ui->seriePickBox->setVisible(true);
    this->ui->setPickBox->setVisible(true);
    this->ui->serieAndSetWidget->setVisible(true);
    this->display_load_series = true;
    this->display_pick_set = false;
    this->display_cards = false;
    this->UnlockCardPanel();

    this->UpdateUi();
}

void MainWindow::SwitchToDeckEditor(){
    this->UnloadData(false);

    this->display_main_menu = false;
    this->display_card_list = false;
    this->display_deck_editor =true;
    this->UnlockCardPanel();
    this->UpdateUi();
    ParseDecks(this->decks);
    this->ui->deckPixBox->clear();
    std::map<std::string,Deck>::iterator it;
    for (it = this->decks.begin(); it != this->decks.end(); it++){
            std::string deck_key = (*it).first;
            Deck &deck = (*it).second;
            this->ui->deckPixBox->addItem(QString::fromStdString(deck.getName()));
    }
}

void MainWindow::PickDeck(){
    this->ClearDeckWidget();
    std::string deck_name = this->ui->deckPixBox->currentText().toStdString();
    //LoadDeck(this->decks,deck_name,this->series);
    this->picked_deck = deck_name;
    std::map<std::string,Deck>::iterator itdeck;
    itdeck = decks.find (deck_name);
    if (itdeck != decks.end()){
        (*itdeck).second.Print();
        if ((*itdeck).second.getCardList().size() == 0){
            LoadDeck(this->decks,deck_name,this->series);
        }
    }
    else {
        LoadDeck(this->decks,deck_name,this->series);
    }
    this->UnlockCardPanel();
    this->DisplayPickedDeck();
}
void MainWindow::UpdateUi(){
    //this->display_load_series = true;
    //this->display_pick_set = false;
    //this->display_cards = false;

    // this->ui->widget->setAttribute(Qt::WA_TransparentForMouseEvents,true); // WHEN DISABLING A WIDGET, PUT THIS : Prevent it to catch mouse event
    // setAttribute(Qt::WA_TransparentForMouseEvents,false);// WHEN ENABLING // WHEN DISABLING A WIDGET, PUT THIS : enable back mouse events

    this->ui->CardListWidget->setVisible(this->display_card_list);
    this->ui->CardListWidget->setAttribute(Qt::WA_TransparentForMouseEvents,!this->display_card_list);

    this->ui->MainMenuWidget->setVisible(this->display_main_menu);
    this->ui->MainMenuWidget->setAttribute(Qt::WA_TransparentForMouseEvents,!this->display_main_menu);

    this->ui->DeckVisualisationWidget->setVisible(this->display_deck_editor);
    this->ui->DeckVisualisationWidget->setAttribute(Qt::WA_TransparentForMouseEvents,!this->display_deck_editor);

    this->ui->simulatorWidget->setVisible(this->display_card_list && this->display_load_series);
    this->ui->serieAndSetWidget->setVisible(this->display_card_list);
    this->ui->serieWidget_2->setVisible(this->display_card_list);
    this->ui->setWidget_2->setVisible(this->display_card_list && this->display_pick_set);
    this->ui->CardWholeAreaWidget_2->setVisible(this->display_card_list && this->display_pick_set);
    this->ui->searchWidget->setVisible(this->display_card_list && this->display_pick_set);

    this->ui->seriePickBox->setVisible(this->display_card_list);
    this->ui->setPickBox->setVisible(this->display_card_list);
    this->ui->serieAndSetWidget->setVisible(this->display_card_list);
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

void MainWindow::RedisplayAfterFilter(){
    this->ClearCardsWidget(false);
    this->ApplyFilter();
    this->SortFilteredCards(&this->current_cards_to_display,this->current_orders);
    this->DisplayFilteredCards();
}


void MainWindow::ApplyFilters(){
    this->RedisplayAfterFilter();
}


void MainWindow::AddCostFilter(bool active,int level){
    if (active){
        // add to filter
        if(std::find(this->current_cost_filters.begin(), this->current_cost_filters.end(),level) == this->current_cost_filters.end()){
            this->current_cost_filters.push_back(level);

        }
    }
    else
    {
        if(std::find(this->current_cost_filters.begin(), this->current_cost_filters.end(),level) != this->current_cost_filters.end()){
            this->current_cost_filters.erase(std::remove(this->current_cost_filters.begin(), this->current_cost_filters.end(), level), this->current_cost_filters.end());
        }
        // remove from filter
    }
}

void MainWindow::AddLevelFilter(bool active,int level){
    if (active){
        // add to filter
        if(std::find(this->current_level_filters.begin(), this->current_level_filters.end(),level) == this->current_level_filters.end()){
            this->current_level_filters.push_back(level);

        }
    }
    else
    {
        if(std::find(this->current_level_filters.begin(), this->current_level_filters.end(),level) != this->current_level_filters.end()){
            this->current_level_filters.erase(std::remove(this->current_level_filters.begin(), this->current_level_filters.end(), level), this->current_level_filters.end());

        }
        // remove from filter


    }
}

void MainWindow::AddTypeFilter(bool active, CardType type){
    if (active){
        // add to filter
        if(std::find(this->current_type_filters.begin(), this->current_type_filters.end(),type) == this->current_type_filters.end()){
            this->current_type_filters.push_back(type);
        }
    }
    else
    {
        if(std::find(this->current_type_filters.begin(), this->current_type_filters.end(),type) != this->current_type_filters.end()){
            this->current_type_filters.erase(std::remove(this->current_type_filters.begin(), this->current_type_filters.end(), type), this->current_type_filters.end());
        }
        // remove from filter


    }
}
void MainWindow::AddColorFilter(bool active,Color color){
    if (active){
        // add to filter
        if(std::find(this->current_color_filters.begin(), this->current_color_filters.end(),color) == this->current_color_filters.end()){
            this->current_color_filters.push_back(color);
        }
    }
    else
    {
        if(std::find(this->current_color_filters.begin(), this->current_color_filters.end(),color) != this->current_color_filters.end()){
            this->current_color_filters.erase(std::remove(this->current_color_filters.begin(), this->current_color_filters.end(), color), this->current_color_filters.end());
        }
        // remove from filter


    }

}

void MainWindow::AddTraitFilter(bool active,std::string trait){
    if (active){
        // add to filter
        if(std::find(this->current_trait_filters.begin(), this->current_trait_filters.end(),trait) == this->current_trait_filters.end()){
            this->current_trait_filters.push_back(trait);

        }
    }
    else
    {
        if(std::find(this->current_trait_filters.begin(), this->current_trait_filters.end(),trait) != this->current_trait_filters.end()){
            this->current_trait_filters.erase(std::remove(this->current_trait_filters.begin(), this->current_trait_filters.end(), trait), this->current_trait_filters.end());

        }
        // remove from filter


    }
}



void MainWindow::TestFiltersAndSorts(){
    std::cout << "test filter and sorts" << std::endl;
    WriteCardsToFile(this->all_cards_available,"/home/benjamin/cards_to_display_before.txt");
     std::cout << "after first save" << std::endl;
    // Try to filter each possibility to only get level 3 red character, than sort on reverse power
    this->current_color_filters.erase(std::remove(this->current_color_filters.begin(), this->current_color_filters.end(), Color::YELLOW), this->current_color_filters.end());
    this->current_color_filters.erase(std::remove(this->current_color_filters.begin(), this->current_color_filters.end(), Color::BLUE), this->current_color_filters.end());
    this->current_type_filters.erase(std::remove(this->current_type_filters.begin(), this->current_type_filters.end(), CardType::EVENT), this->current_type_filters.end());
    this->current_type_filters.erase(std::remove(this->current_type_filters.begin(), this->current_type_filters.end(), CardType::CLIMAX), this->current_type_filters.end());
    this->current_level_filters.erase(std::remove(this->current_level_filters.begin(), this->current_level_filters.end(), 0), this->current_level_filters.end());
    this->current_level_filters.erase(std::remove(this->current_level_filters.begin(), this->current_level_filters.end(), 1), this->current_level_filters.end());
    this->current_level_filters.erase(std::remove(this->current_level_filters.begin(), this->current_level_filters.end(), 2), this->current_level_filters.end());

    this->current_orders.erase(std::remove(this->current_orders.begin(), this->current_orders.end(), Orders::POWER_ASCENDING), this->current_orders.end());
    this->current_orders.push_back(Orders::POWER_DESCENDING);

    std::cout << "Current color filters : " << std::endl;
    for (Color c : this->current_color_filters){
        std::cout << GetColorString(c) << std::endl;
    }
    this->FillCards();
    this->ApplyFilter();
    this->SortFilteredCards(&this->current_cards_to_display,this->current_orders);

    std::cout << "Found " << this->current_cards_to_display.size() << " red only card cards " << std::endl;
    for (Card c2 : this->current_cards_to_display){
        std::cout << c2.getName() << std::endl;
        std::cout << "Card level " << c2.getLevel() << " color " << GetColorString(c2.getColor()) << std::endl;
    }
    WriteCardsToFile(this->current_cards_to_display,"/home/benjamin/cards_to_display_after.txt");
}

 QColor GetQColorFromCardColor(Color c){
    if (c == Color::GREEN){
        return QColorConstants::Svg::green;
    }

    if (c == Color::BLUE){
        return QColorConstants::Svg::blue;
    }

    if (c == Color::PURPLE){
        return QColorConstants::Svg::purple;
    }

    if (c == Color::RED){
        return QColorConstants::Svg::red;
    }

    if (c == Color::YELLOW)
    {
        return QColorConstants::Svg::yellow;
    }

    return QColorConstants::Svg::white;
}
void MainWindow::FillFiltersUsingSet(){
    if (this->choosen_sets.size() == 0){
        return;
    }



    this->available_level_filters = {}; // This will be loaded when laoding a set cards
    this->available_cost_filters = {};
    this->available_color_filters = {};
    this->current_level_filters = {};
    this->current_cost_filters = {};
    this->current_color_filters = {};
    this->available_type_filters = {};
    this->current_type_filters = {};

    this->available_trait_filters = {};
    this->current_trait_filters = {};

    for (int i {0}; i < this->choosen_sets.size(); ++i){

        std::map<std::string,Card>::iterator it;
        for (it = this->choosen_sets.at(i)->getCards().begin(); it != this->choosen_sets.at(i)->getCards().end(); it++){
            std::string code = (*it).first;
            Card c  = (*it).second;
            if(std::find(this->available_level_filters.begin(), this->available_level_filters.end(), c.getLevel()) == this->available_level_filters.end()) {
                this->available_level_filters.push_back(c.getLevel());
                this->current_level_filters.push_back(c.getLevel());
            }

            if(std::find(this->available_color_filters.begin(), this->available_color_filters.end(), c.getColor()) == this->available_color_filters.end()) {
                this->available_color_filters.push_back(c.getColor());
                this->current_color_filters.push_back(c.getColor());
            }

            if(std::find(this->available_cost_filters.begin(), this->available_cost_filters.end(), c.getCost()) == this->available_cost_filters.end()) {
                this->available_cost_filters.push_back(c.getCost());
                this->current_cost_filters.push_back(c.getCost());
            }

            if(std::find(this->available_type_filters.begin(), this->available_type_filters.end(), c.getCardType()) == this->available_type_filters.end()) {
                this->available_type_filters.push_back(c.getCardType());
                this->current_type_filters.push_back(c.getCardType());
            }
            for (std::string trait : c.getTraits()){
                if(std::find(this->available_trait_filters.begin(), this->available_trait_filters.end(), trait) == this->available_trait_filters.end()) {
                    this->available_trait_filters.push_back(trait);
                    this->current_trait_filters.push_back(trait);
                }
            }
            if (c.getColor() == Color::NONE){
                c.print();
                return;
            }
        }
    }


    std::sort(this->available_level_filters.begin(),this->available_level_filters.end());
    std::sort(this->available_cost_filters.begin(),this->available_cost_filters.end());
    std::sort(this->available_color_filters.begin(),this->available_color_filters.end());
    std::sort(this->current_level_filters.begin(),this->current_level_filters.end());
    std::sort(this->current_cost_filters.begin(),this->current_cost_filters.end());
    std::sort(this->current_color_filters.begin(),this->current_color_filters.end());
    std::sort(this->available_type_filters.begin(),this->available_type_filters.end());
    std::sort(this->current_type_filters.begin(),this->current_type_filters.end());
    std::sort(this->available_trait_filters.begin(),this->available_trait_filters.end());
    std::sort(this->current_trait_filters.begin(),this->current_trait_filters.end());

    std::cout << "Available level filters"  << std::endl;
    for (int l : this->available_level_filters){
        QCheckBox* checkbox = new QCheckBox(QString("%1").arg(l));
        this->ui->levelFilterLayout_2->addWidget(checkbox);
        checkbox->setChecked(true);
        QObject::connect(checkbox, &QCheckBox::checkStateChanged, [l,this](int state) {
            bool checked = (state == Qt::Checked);
            // Call your function with the checked state and the value
            this->AddLevelFilter(checked, l);
        });
        std::cout << l << " , ";
    }
    this->ui->groupBox_2->setLayout(this->ui->levelFilterLayout_2);

    std::cout << std::endl;


    std::cout << "Available trait filters"  << std::endl;
    for (std::string l : this->available_trait_filters){
        QCheckBox* checkbox = new QCheckBox(QString::fromStdString(l));
        this->ui->traitFilterLayout->addWidget(checkbox);
        checkbox->setChecked(true);
        QObject::connect(checkbox, &QCheckBox::checkStateChanged, [l,this](int state) {
            bool checked = (state == Qt::Checked);
            // Call your function with the checked state and the value
            this->AddTraitFilter(checked, l);
        });
        std::cout << l << " , ";
    }
    this->ui->groupBox_5->setLayout(this->ui->traitFilterLayout);

    std::cout << std::endl;


    std::cout << "Available type filters"  << std::endl;
    for (CardType l : this->available_type_filters){
        QCheckBox* checkbox = new QCheckBox(QString::fromStdString(GetCardTypeString(l)));
        this->ui->typeFilterLayout->addWidget(checkbox);
        checkbox->setChecked(true);
        QObject::connect(checkbox, &QCheckBox::checkStateChanged, [l,this](int state) {
            bool checked = (state == Qt::Checked);
            // Call your function with the checked state and the value
            this->AddTypeFilter(checked, l);
        });
        std::cout << GetCardTypeString(l) << " , ";
    }
    this->ui->groupBox_4->setLayout(this->ui->typeFilterLayout);
    std::cout << std::endl;


    std::cout << "Available color filters"  << std::endl;
    for (Color l : this->available_color_filters){
        QCheckBox* checkbox = new QCheckBox(QString::fromStdString(GetColorString(l)));
        this->ui->colorFilterLayout_2->addWidget(checkbox);
        checkbox->setChecked(true);
        QPalette p = checkbox->palette();
        p.setColor(QPalette::Active, QPalette::WindowText, GetQColorFromCardColor(l));
        checkbox->setPalette(p);
        QObject::connect(checkbox, &QCheckBox::checkStateChanged, [l,this](int state) {
            bool checked = (state == Qt::Checked);
            // Call your function with the checked state and the value
            this->AddColorFilter(checked, l);
        });
        std::cout << GetColorString(l) << " , ";
    }
    this->ui->groupBox_3->setLayout(this->ui->colorFilterLayout_2);
    std::cout << std::endl;

    std::cout << "Current color filters"  << std::endl;
    for (Color l : this->current_color_filters){
        std::cout <<  GetColorString(l) << " , ";
    }
    std::cout << std::endl;


    std::cout << "Available cost filters"  << std::endl;

    for (int l : this->available_cost_filters){
        QCheckBox* checkbox = new QCheckBox(QString("%1").arg(l));
        this->ui->costFilterLayout_2->addWidget(checkbox);
        checkbox->setChecked(true);
        QObject::connect(checkbox, &QCheckBox::checkStateChanged, [l,this](int state) {
            bool checked = (state == Qt::Checked);
            // Call your function with the checked state and the value
            this->AddCostFilter(checked, l);
        });
        std::cout << l << " , ";
    }
    this->ui->groupBox->setLayout(this->ui->costFilterLayout_2);
    std::cout << std::endl;

    std::cout << "Current cost filters"  << std::endl;
    for (int l : this->current_cost_filters){
        std::cout << l << " , ";
    }
    std::cout << std::endl;


}

int MainWindow::getGridCount(){
    return this->grid_height*this->grid_width;
}


std::vector<Serie> MainWindow::getAllSeries(){
    return this->series;
}
Serie MainWindow::getSerieByName(std::string name){
    for (int i{ 0 }; i <= this->series.size(); ++i){
        if (this->series.at(i).getName() == name){
            return this->series.at(i);
        }
    }
    return Serie("","");
}

void MainWindow::OnSeriePick(){
    this->UnloadData(false);
    std::string choosen_serie_name = this->ui->seriePickBox->currentText().toStdString();
    //QMessageBox::information(this, "Item Selection",
    //                         this->m_series_dropdown->currentText());
    this->display_pick_set = true;
    this->UpdateUi();
    this->ui->setPickBox->clear();
    for (int i {0}; i < this->series.size(); ++i){
        if (this->series.at(i).getName() ==  choosen_serie_name){
            this->choosen_serie = &this->series.at(i);
        }

    }

    if (this->choosen_serie != nullptr){
        for (Set se : this->choosen_serie->getAllSets()){
            this->ui->setPickBox->addItem(QString::fromStdString(se.getName()));
        }
    }
    else{
        std::cout << "Error, serie not found among the list : " << this->ui->seriePickBox->currentText().toStdString() << std::endl;
    }

}

void MainWindow::FillCards(){
    this->all_cards_available = {};
        for (int i {0}; i < this->choosen_sets.size(); ++i){
            std::map<std::string,Card>::iterator it;
            for (it = this->choosen_sets.at(i)->getCards().begin(); it != this->choosen_sets.at(i)->getCards().end(); it++){
                std::string code = (*it).first;
                Card c  = (*it).second;
                this->all_cards_available.push_back(c);
            }
        }
}

void MainWindow::ApplyFilter(){
    this->current_cards_to_display = {};
    for (Card c : this->all_cards_available){
        // apply filter by not adding filtered cards
        if (this->ui->searchFilterEdit->toPlainText() != ""){
            if (findSubstringIgnoreCase(this->ui->searchFilterEdit->toPlainText().toStdString(),c.getName())){
                this->current_cards_to_display.push_back(c);
            }
            else if (findSubstringIgnoreCase(this->ui->searchFilterEdit->toPlainText().toStdString(),c.getKey()))
            {
                this->current_cards_to_display.push_back(c);
            }
            else if (findSubstringIgnoreCase(this->ui->searchFilterEdit->toPlainText().toStdString(),c.getText()))
            {
                this->current_cards_to_display.push_back(c);
            }
        }
        else if (std::find(this->current_level_filters.begin(), this->current_level_filters.end(), c.getLevel()) != this->current_level_filters.end() &&
            std::find(this->current_cost_filters.begin(), this->current_cost_filters.end(), c.getCost()) != this->current_cost_filters.end() &&
            std::find(this->current_color_filters.begin(), this->current_color_filters.end(), c.getColor()) != this->current_color_filters.end() &&
            std::find(this->current_type_filters.begin(), this->current_type_filters.end(), c.getCardType()) != this->current_type_filters.end())
        {
            bool include_due_to_traits = false;
            for (std::string trait : c.getTraits()){
                if (std::find(this->current_trait_filters.begin(), this->current_trait_filters.end(), trait) != this->current_trait_filters.end()){
                    include_due_to_traits = true;
                }
            }

            if (include_due_to_traits || this->available_trait_filters.size() == 0){
                this->current_cards_to_display.push_back(c);
            }
        }
    }
}


void MainWindow::SortFilteredCards(std::vector<Card>* cards_to_sort,std::vector<Orders> order){
    std::sort(cards_to_sort->begin(), cards_to_sort->end(), [order](Card a, Card b) { // lambda func
        if (order.size() > 0){
            for (Orders o : order){
                if (o == Orders::TYPE_ASCENDING){
                    if (a.getCardType() != b.getCardType()){
                        return a.getCardType() < b.getCardType();
                    }
                }
                else if (o == Orders::COLOR){
                    if (a.getColor() != b.getColor()){
                        return a.getColor() < b.getColor();
                    }
                }
                else
                {
                    if (a.getCardType() != b.getCardType()){
                        return a.getCardType() < b.getCardType();
                    }
                }

                if (o == Orders::LEVEL_ASCENDING){
                    if (a.getLevel() != b.getLevel()){
                        return a.getLevel() < b.getLevel();
                    }
                }
                else if (o == Orders::LEVEL_DESCENDING){
                    if (a.getLevel() != b.getLevel()){
                        return a.getLevel() > b.getLevel();
                    }
                }

                else if (o == Orders::COST_ASCENDING){
                    if (a.getCost() != b.getCost()){
                        return a.getCost() < b.getCost();
                    }
                }
                else if (o == Orders::COST_DESCENDING){
                    if (a.getCost() != b.getCost()){
                        return a.getCost() > b.getCost();
                    }
                }

                else if (o == Orders::POWER_ASCENDING){
                    if (a.getPower() != b.getPower()){
                        return a.getPower() < b.getPower();
                    }
                }
                else if (o == Orders::POWER_DESCENDING){
                    if (a.getPower() != b.getPower()){
                        return a.getPower() > b.getPower();
                    }
                }
                else if (o == Orders::KEYCODE_ASCENDING){
                    if (a.getKey() != b.getKey()){
                        return a.getKey() < b.getKey();
                    }
                }

            }
        }
        if (a.getLevel() != b.getLevel()){ // default is level > power
            return a.getLevel() < b.getLevel();
        }
        else {
            return a.getPower() > b.getPower(); // default is power but should not happen
        }
    });
}
void MainWindow::OnSetPick(){
    this->UnloadData(false);

    std::string choosen_set_name = this->ui->setPickBox->currentText().toStdString();
    this->display_cards = true;
    this->UpdateUi();
    this->choosen_sets.clear();
    if (this->choosen_serie != nullptr){
        for (int i {0}; i < this->choosen_serie->getAllSets().size(); ++i){
            if (this->choosen_serie->getAllSets().at(i).getName() ==  choosen_set_name){
                this->choosen_sets.push_back(&this->choosen_serie->getAllSets().at(i));
            }

        }
    }
    this->FillFiltersUsingSet();
    this->FillCards();
    this->ApplyFilter();
    this->SortFilteredCards(&this->current_cards_to_display,this->current_orders);
    this->DisplayFilteredCards();


}


void MainWindow::OnSerieCardsPick(){
    this->UnloadData(false);
    std::string choosen_serie_name = this->ui->seriePickBox->currentText().toStdString();
    //QMessageBox::information(this, "Item Selection",
    //                         this->m_series_dropdown->currentText());
    this->display_pick_set = true;
    this->display_cards = true;
    this->UpdateUi();
    this->ui->setPickBox->clear();
    this->choosen_sets.clear();
    for (int i {0}; i < this->series.size(); ++i){
        if (this->series.at(i).getName() ==  choosen_serie_name){
            this->choosen_serie = &this->series.at(i);
        }

    }

    if (this->choosen_serie != nullptr){
        for (Set se : this->choosen_serie->getAllSets()){
            this->ui->setPickBox->addItem(QString::fromStdString(se.getName()));
        }
    }
    else{
        std::cout << "Error, serie not found among the list : " << this->ui->seriePickBox->currentText().toStdString() << std::endl;
    }

    if (this->choosen_serie != nullptr){
        for (int i {0}; i < this->choosen_serie->getAllSets().size(); ++i){
            this->choosen_sets.push_back(&this->choosen_serie->getAllSets().at(i));
        }
    }

    this->FillFiltersUsingSet();
    this->FillCards();
    this->ApplyFilter();
    this->SortFilteredCards(&this->current_cards_to_display,this->current_orders);
    this->DisplayFilteredCards();
}


void MainWindow::DisplayPickedDeck(){
    //std::cout << "Display picked : " << this->picked_deck << std::endl;
    int row = 0, col = 0;
    const int columns = 10;
    std::map<std::string,Deck>::iterator itdeck;
    itdeck = decks.find (this->picked_deck);
    if (itdeck != decks.end()){
        Deck de = (*itdeck).second;
        std::vector<Orders> deck_orders = {Orders::TYPE_ASCENDING,Orders::LEVEL_ASCENDING,Orders::COST_ASCENDING,Orders::POWER_ASCENDING,Orders::COLOR};
        std::vector<Card> card_list = (*itdeck).second.getCardList();
        this->SortFilteredCards(&card_list,deck_orders);
        for (Card value : card_list){
            //std::cout << "for loop card : " << value.getKey() << std::endl;
            std::string path_str = value.getImagePath();
            //std::cout << "found image : " << value.getKey() << std::endl;

            //path_str.replace(path_str.find(" "),std::string(" ").size(),"\ ");
            QString path_image = QString::fromStdString(path_str);
            //std::cout << value.getKey() << std::endl;
            if (QFile::exists(path_image)) {
                QPixmap pix(path_image);
                HoverLabel* imgLabel = new HoverLabel(this);
                imgLabel->setScaledContents( true );
                imgLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
                imgLabel->setPixmap(pix);
                connect(imgLabel,&HoverLabel::mouseEntered, [this,value]() {
                    this->ShowCardToSidePanel(value);
                });
                connect(imgLabel,&HoverLabel::mouseClicked, [this,value]() {
                    this->LockCardPanel(value);
                });
                connect(imgLabel,&HoverLabel::mouseDoubleClicked, [this]() {
                    this->UnlockCardPanel();
                });
                //QObject::connect(mapper,SIGNAL(mapped(QWidget *)),this,SLOT(mySlot(QWidget *)));

                //QObject::connect(imgLabel, SIGNAL(clicked()),mapper,SLOT(ShowCardToSidePanel(value)));
                //mapper->setMapping(imgLabel, imgLabel);

                //QSignalMapper* signalmapper = new QSignalMapper();
                //connect (imgLabel, SIGNAL(triggered()), signalmapper, SLOT(map())) ;
                //signalmapper ->setMapping (imgLabel, value);
                //connect (signalmapper , SIGNAL(mapped(Card)), this, SLOT(ShowCardToSidePanel(Card)));
                //connect(imgLabel, SIGNAL(clicked()),this,[this,value](){this->ShowCardToSidePanel(value);});
                //imgLabel->setPixmap(pix.scaled(220, 310,Qt::KeepAspectRatio, Qt::SmoothTransformation));
                imgLabel->setFixedSize(154, 213); // add padding around thumbnail
                this->ui->cardGrid->addWidget(imgLabel,row,col);
                if (++col >= columns) {
                    col = 0;
                    ++row;
                }
            }
        }
    }

}

void MainWindow::UnlockCardPanel(){
    this->lock_card_panel = false;
    this->ui->cardImage->setStyleSheet(QString(""));

}

void MainWindow::LockCardPanel(Card c){
    this->lock_card_panel = false;
    this->ShowCardToSidePanel(c);
    this->lock_card_panel = true;
    this->ui->cardImage->setStyleSheet(QString("border-style:solid;border-width:2px;border-color:red"));
}

void MainWindow::ShowCardToSidePanel(Card c){
    if (!this->lock_card_panel){
        std::string path_str = c.getImagePath();
        QString path_image = QString::fromStdString(path_str);
        if (QFile::exists(path_image)) {
            QPixmap pix(path_image);
            this->ui->cardImage->setScaledContents( true );
            this->ui->cardImage->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
            this->ui->cardImage->setPixmap(pix);
        }
        this->ui->cardText->setText(QString::fromStdString(c.getCardHTML()));
    }
}

void MainWindow::DisplayFilteredCards(){

    this->ui->cardGridWidget_2->setWordWrap(true);
    this->ui->cardGridWidget_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->ui->cardGridWidget_2->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    for (Card c : this->current_cards_to_display){
        //std::cout << c.getImagePath() << std::endl;
        if (c.getImagePath() != "" ){
            std::string path_str = c.getImagePath();
            //path_str.replace(path_str.find(" "),std::string(" ").size(),"\ ");
            QString path_image = QString::fromStdString(path_str);
            QWidget* final_widget = new QWidget();
            QHBoxLayout* qblayout = new QHBoxLayout(final_widget);
            if (QFile::exists(path_image)) {
                QPixmap pix(path_image);
                QLabel* textLabel = new QLabel(QString::fromStdString(c.getCardHTML()));
                QLabel* imgLabel = new QLabel();
                textLabel->setWordWrap(true);
                //textLabel->setIndent(224);
                textLabel->setTextFormat(Qt::RichText);
                imgLabel->setScaledContents( true );
                imgLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
                imgLabel->setPixmap(pix);
                imgLabel->setFixedSize(224, 313); // add padding around thumbnail
                textLabel->setMinimumHeight(313);
                textLabel->setMinimumWidth(800);
                textLabel->setMaximumWidth(1300);
                textLabel->setAlignment(Qt::AlignLeft);
                qblayout->setAlignment(Qt::AlignLeft);
                qblayout->addWidget(imgLabel);
                qblayout->addWidget(textLabel);
                //qblayout->addStretch(1);
                //qblayout->setSpacing(0);
                //imgLabel->setAlignment(Qt::AlignCenter);*/
                final_widget->setFixedHeight(313);
                QListWidgetItem *item = new QListWidgetItem();
                this->ui->cardGridWidget_2->addItem(item);
                this->ui->cardGridWidget_2->setItemWidget(item,final_widget);
                item->setSizeHint( final_widget->sizeHint() );
                this->cards_items.push_back(item);
                //this->ui->cardGridWidget_2->addWidget(imgLabel);
            } else {
                qWarning() << "Image not found:" << path_image;
            }

        }

    }

    //this->ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //this->ui->scrollArea->setLayout(this->ui->CardWholeAreaWidget_2);
    this->ui->cardGridWidget_2->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
    //this->ui->scrollArea->verticalScrollBar()->setValue(this->ui->scrollArea->verticalScrollBar()->value() + 10);

    //this->ui->cardGridWidget_2->setFixedSize(ui->cardGridWidget_2->sizeHintForColumn(0) + ui->cardGridWidget_2->frameWidth() * 2,
    //                             ui->cardGridWidget_2->sizeHintForRow(0) * ui->cardGridWidget_2->count() + 2 * ui->cardGridWidget_2->frameWidth());

}


void MainWindow::OnExit(){
    QApplication::quit();
}

void MainWindow::OnUnloadSimulator(){
    DeleteSetting("simulator_data_path");
    this->SwitchToMainMenu();
    this->UnloadData();
    this->ui->simulatorWidget->setVisible(true);
    this->ui->simulatorWidget->setAttribute(Qt::WA_TransparentForMouseEvents,false);
    this->ui->MenusWidget->setVisible(false);
    this->ui->MenusWidget->setAttribute(Qt::WA_TransparentForMouseEvents,true);
}

void MainWindow::UnloadData(bool unload_set_widget){
    this->ClearCardsWidget();
    this->ClearDeckWidget();

    this->available_level_filters.clear(); // Clear memory of objects in the vector
    this->available_level_filters = {}; // completely reset the size of the vector itself
    this->available_cost_filters.clear(); // Clear memory of objects in the vector
    this->available_cost_filters = {}; // completely reset the size of the vector itself
    this->available_color_filters.clear(); // Clear memory of objects in the vector
    this->available_color_filters = {}; // completely reset the size of the vector itself
    this->available_type_filters.clear(); // Clear memory of objects in the vector
    this->available_type_filters = {}; // completely reset the size of the vector itself
    this->available_trait_filters.clear(); // Clear memory of objects in the vector
    this->available_trait_filters = {}; // completely reset the size of the vector itself

    this->current_level_filters.clear(); // Clear memory of objects in the vector
    this->current_level_filters = {}; // completely reset the size of the vector itself
    this->current_cost_filters.clear(); // Clear memory of objects in the vector
    this->current_cost_filters = {}; // completely reset the size of the vector itself
    this->current_type_filters.clear(); // Clear memory of objects in the vector
    this->current_type_filters = {}; // completely reset the size of the vector itself
    this->current_color_filters.clear(); // Clear memory of objects in the vector
    this->current_color_filters = {}; // completely reset the size of the vector itself
    this->current_trait_filters.clear(); // Clear memory of objects in the vector
    this->current_trait_filters = {}; // completely reset the size of the vector itself

    this->current_orders.clear();
    this->current_orders = {Orders::KEYCODE_ASCENDING};

    this->ui->searchFilterEdit->setPlainText(QString(""));



    if (unload_set_widget){
        this->choosen_serie = nullptr;
        this->choosen_sets  = {};
        this->series.clear();
        this->series = {};
        this->display_load_series = true;
        this->display_pick_set = false;
        this->display_cards = false;


        this->ui->simulatorPathBox->clear();
        this->ui->seriePickBox->clear();
        this->ui->setPickBox->clear();
    }
    this->current_cards_to_display.clear();
    this->current_cards_to_display = {};


    this->UpdateUi();

}

void clearLayout(QLayout* layout) {
    if (!layout)
        return;

    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();  // Schedule widget for deletion
        }
        if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout);  // Recursively clear nested layouts
            delete childLayout;
        }
        delete item;
    }
}

void MainWindow::ClearDeckWidget(){

    clearLayout(this->ui->cardGrid);

    this->ui->cardImage->setScaledContents( true );
    this->ui->cardImage->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    this->ui->cardImage->clear();

    this->ui->cardText->clear();
    this->ui->cardText->setWordWrap(true);
    //textLabel->setIndent(224);
    this->ui->cardText->setTextFormat(Qt::RichText);
    this->ui->cardText->setMaximumWidth(1300);
    this->ui->cardText->setAlignment(Qt::AlignLeft);

    //this->ui->deckPixBox->clear();
}
void MainWindow::ClearCardsWidget(bool clear_filters){
    /*if (this->cards_items.size() > 0){
        for (QListWidgetItem* item : this->cards_items){
            this->ui->cardGridWidget_2->removeItemWidget(item);
            delete item;
        }
    }*/

    for (int i = 0; i < this->ui->cardGridWidget_2->count(); ++i) {
        QListWidgetItem* item = this->ui->cardGridWidget_2->item(i);
        QWidget* widget = this->ui->cardGridWidget_2->itemWidget(item);
        if (widget) {
            delete widget;  // Delete the associated custom widget
        }
        delete item;  // Delete the item itself
    }
    this->ui->cardGridWidget_2->clear();
    if (clear_filters){
        clearLayout(this->ui->costFilterLayout_2);
        clearLayout(this->ui->colorFilterLayout_2);
        clearLayout(this->ui->typeFilterLayout);
        clearLayout(this->ui->traitFilterLayout);
        clearLayout(this->ui->levelFilterLayout_2);
    }
    /*QLayoutItem* item;
    while ( ( item =  this->ui->costFilterLayout_2->layout()->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
    this->ui->costFilterLayout_2->clear();
    //delete  this->ui->costFilterLayout_2->layout();*/

    /*QLayoutItem* item2;
    while ( ( item2 =  this->ui->colorFilterLayout_2->layout()->takeAt( 0 ) ) != NULL )
    {
        delete item2->widget();
        delete item2;
    }*/



    /*QLayoutItem* item3;
    while ( ( item3 =  this->ui->typeFilterLayout->layout()->takeAt( 0 ) ) != NULL )
    {
        delete item3->widget();
        delete item3;
    }*/


    /*QLayoutItem* item4;
    while ( ( item4 =  this->ui->traitFilterLayout->takeAt( 0 ) ) != nullptr )
    {
        delete item4->widget();
        delete item4;
    }
    //delete  this->ui->traitFilterLayout->layout();
    */
    /*
    QLayoutItem* item5;
    while ( ( item5 =  this->ui->levelFilterLayout_2->takeAt( 0 ) ) != nullptr )
    {
        delete item5->widget();
        delete item5;
    }*/


    this->cards_items = {};
}

void MainWindow::LoadSimulator(std::string simulator_path){
    if (fs::exists(simulator_path)){
        ParseCommonEffects(simulator_path);
        // internal subfolders
        std::string cards_folder = simulator_path+"StreamingAssets"+separator()+"Cards";
        // parsing all series
        this->series = ParseSeries(cards_folder);


        for (Serie sa : this->series){
            /*std::cout << sa.getName() << " : " << sa.getAllSets().size() << std::endl;
            for (Set set_obj : sa.getAllSets()){
                std::cout << set_obj.getName() << " , " << set_obj.getKey() << " : " << set_obj.getCards().size() << std::endl;
            }*/
            this->ui->seriePickBox->addItem(QString::fromStdString(sa.getName()));
        }
        this->display_load_series = false;
        this->ui->simulatorWidget->setVisible(false);
        this->ui->simulatorWidget->setAttribute(Qt::WA_TransparentForMouseEvents,true);
        this->ui->MenusWidget->setVisible(true);
        this->ui->MenusWidget->setAttribute(Qt::WA_TransparentForMouseEvents,false);
        this->SwitchToMainMenu();
        this->UpdateUi();
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
