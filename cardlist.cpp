#include "cardlist.h"
#include "ui_cardlist.h"
#include "dataloader.h"
#include <QUiLoader>
#include <QFile>
#include <QStandardItemModel>
#include <QStandardItem>
#include <iostream>
#include <QCheckBox>
#include <QStringList>
#include <QPushButton>
cardlist::cardlist(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::cardlist)
{
    ui->setupUi(this);
    std::vector<Serie*>* series = DataLoader::GetInstance()->getSeries();


    for (Serie* sa : *series){
        this->ui->seriePickBox->addItem(QString::fromStdString(sa->getName()));
    }
    this->ui->seriePickBox->setVisible(true);
    this->ui->setPickBox->setVisible(false);
    this->ui->setLoadButton->setVisible(false);
    this->ui->searchWidget->setVisible(false);
    this->ui->serieAndSetWidget->setVisible(true);
    this->ui->CardWholeAreaWidget_2->setVisible(false);
    connect(this->ui->serieLoadButton, SIGNAL (clicked(bool)), this, SLOT (OnSeriePick()));
    connect(this->ui->setLoadButton, SIGNAL (clicked(bool)), this, SLOT (OnSetPick()));
    connect(this->ui->serieLoadCardsButton,SIGNAL (clicked(bool)),this, SLOT (OnSerieCardsPick()));
    connect(this->ui->ApplyFiltersButton, SIGNAL (clicked(bool)),this, SLOT (ApplyFilters()));
    connect(this->ui->searchButton, SIGNAL (clicked(bool)),this, SLOT (ApplyFilters()));
    connect(this->ui->backButton, SIGNAL (clicked(bool)),this, SLOT (SwitchToMainMenu()));
    connect(this->ui->selectAllTrait, &QPushButton::clicked,this, [this]() {this->ResetTraitComboBox(true);});
    connect(this->ui->unselectAllTrait, &QPushButton::clicked,this, [this]() {this->ResetTraitComboBox(false);});
    connect(this->ui->selectAllCost, &QPushButton::clicked,this, [this]() {this->ResetCostComboBox(true);});
    connect(this->ui->unselectAllCost, &QPushButton::clicked,this, [this]() {this->ResetCostComboBox(false);});
    connect(this->ui->selectAllLevel, &QPushButton::clicked,this, [this]() {this->ResetLevelComboBox(true);});
    connect(this->ui->unselectAllLevel, &QPushButton::clicked,this, [this]() {this->ResetLevelComboBox(false);});
    connect(this->ui->selectAllType, &QPushButton::clicked,this, [this]() {this->ResetTypeComboBox(true);});
    connect(this->ui->unselectAllType, &QPushButton::clicked,this, [this]() {this->ResetTypeComboBox(false);});
    connect(this->ui->selectAllColor, &QPushButton::clicked,this, [this]() {this->ResetColorComboBox(true);});
    connect(this->ui->unselectAllColor, &QPushButton::clicked,this, [this]() {this->ResetColorComboBox(false);});

    connect(this->ui->selectAllTrigger, &QPushButton::clicked,this, [this]() {this->ResetTriggerComboBox(true);});
    connect(this->ui->unselectAllTrigger, &QPushButton::clicked,this, [this]() {this->ResetTriggerComboBox(false);});
    this->picked_serie = false;
    this->picked_sets = false;

    this->currentCardsImages={};
    this->current_cards_to_display = {};

    this->cards_items = {};
    this->available_level_filters = {}; // This will be loaded when laoding a set cards
    this->available_cost_filters = {};
    this->available_color_filters = {};
    this->available_trait_filters = {};
    this->available_type_filters = {};
    this->available_trigger_filters = {};
    this->all_cards_available = {};


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
    this->current_trigger_filters = {};

}


cardlist::~cardlist()
{
    delete ui;
}

void cardlist::UpdateUi(){
    this->ui->serieAndSetWidget->setVisible(true);
    this->ui->serieWidget_2->setVisible(true);
    this->ui->setWidget_2->setVisible(this->picked_serie);
    this->ui->CardWholeAreaWidget_2->setVisible(this->picked_sets);
    this->ui->searchWidget->setVisible(this->picked_sets);
    this->ui->setLoadButton->setVisible(this->picked_serie);
    this->ui->ApplyFiltersButton->setVisible(this->picked_sets);
    this->ui->seriePickBox->setVisible(true);
    this->ui->setPickBox->setVisible(this->picked_serie);
    this->ui->serieAndSetWidget->setVisible(true);
}

void cardlist::ClearCardsWidget(bool clear_filters){
    for (int i = 0; i < this->ui->cardGridWidget_2->count(); ++i) {
        QListWidgetItem* item = this->ui->cardGridWidget_2->item(i);
        QWidget* widget = this->ui->cardGridWidget_2->itemWidget(item);
        if (widget) {
            delete widget;  // Delete the associated custom widget
        }
        delete item;  // Delete the item itself
    }

    this->ui->cardGridWidget_2->clear();
    this->cards_items = {};
}

void cardlist::UnloadData(bool unload_set_widget){
    this->ClearCardsWidget();

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
        this->picked_serie = false;
        this->picked_sets = false;

        this->ui->seriePickBox->clear();
        this->ui->setPickBox->clear();
    }
    this->current_cards_to_display.clear();
    this->current_cards_to_display = {};


    this->UpdateUi();

}

void cardlist::RedisplayAfterFilter(){
    this->ClearCardsWidget(false);
    this->ApplyFilter();
    SortFilteredCards(&this->current_cards_to_display,this->current_orders);
    this->DisplayFilteredCards();
}


void cardlist::SwitchToMainMenu(){
    MainWindow* mw = getMainWindow();
    mw->SwitchToMainMenu();
}


void cardlist::ApplyFilters(){
    this->RedisplayAfterFilter();
}


void cardlist::AddCostFilter(bool active,QStringList checked_costs){
    this->current_cost_filters = {};
    for (QString cost : checked_costs){
        if(std::find(this->current_cost_filters.begin(), this->current_cost_filters.end(),std::stoi(cost.toStdString())) == this->current_cost_filters.end()){
            this->current_cost_filters.push_back(std::stoi(cost.toStdString()));

        }
    }
}

void cardlist::AddTriggerFilter(bool active,QStringList checked_triggers){
    this->current_trigger_filters = {};
    for (QString cost : checked_triggers){
        if(std::find(this->current_trigger_filters.begin(), this->current_trigger_filters.end(),GetStringTrigger(cost.toStdString())) == this->current_trigger_filters.end()){
            this->current_trigger_filters.push_back(GetStringTrigger(cost.toStdString()));

        }
    }
}

void cardlist::AddLevelFilter(bool active,QStringList checked_levels){
    this->current_level_filters = {};
    for (QString lvl : checked_levels){
        if(std::find(this->current_level_filters.begin(), this->current_level_filters.end(),std::stoi(lvl.toStdString())) == this->current_level_filters.end()){
            this->current_level_filters.push_back(std::stoi(lvl.toStdString()));

        }
    }
}

void cardlist::AddColorFilter(bool active,QStringList checked_colors){
    this->current_color_filters = {};
    for (QString lvl : checked_colors){
        if(std::find(this->current_color_filters.begin(), this->current_color_filters.end(),GetStringColor(lvl.toStdString())) == this->current_color_filters.end()){
            this->current_color_filters.push_back(GetStringColor(lvl.toStdString()));

        }
    }
}


void cardlist::AddTypeFilter(bool active,QStringList checked_types){
    this->current_type_filters = {};
    for (QString lvl : checked_types){
        if(std::find(this->current_type_filters.begin(), this->current_type_filters.end(),GetStringCardType(lvl.toStdString())) == this->current_type_filters.end()){
            this->current_type_filters.push_back(GetStringCardType(lvl.toStdString()));

        }
    }
}




void cardlist::AddTraitFilter(bool active,QStringList checked_traits){
    this->current_trait_filters = {};
    for (QString trait : checked_traits){
        if(std::find(this->current_trait_filters.begin(), this->current_trait_filters.end(),trait.toStdString()) == this->current_trait_filters.end()){
            this->current_trait_filters.push_back(trait.toStdString());

        }
    }
}

QStringList getCheckedItems(QStandardItemModel *model) {
    QStringList selected;
    for (int i = 0; i < model->rowCount(); ++i) {
        QStandardItem *item = model->item(i);
        if (item->checkState() == Qt::Checked) {
            selected << item->text();
        }
    }
    return selected;
}

void cardlist::ResetTriggerComboBox(bool select){
    QListView *listView = new QListView(this->ui->triggersComboBox);
    this->ui->triggersComboBox->setView(listView);

    QStandardItemModel *model = new QStandardItemModel(this->ui->triggersComboBox);
    this->ui->triggersComboBox->setModel(model);
    for (int i = 0; i < this->available_trigger_filters.size(); i++)
    {
        QStandardItem *item = new QStandardItem(QString("").fromStdString(GetTriggerString(this->available_trigger_filters[i])));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData((select)?Qt::Checked:Qt::Unchecked, Qt::CheckStateRole);
        model->appendRow(item);
    }

    connect(model, &QStandardItemModel::itemChanged, this, [=]() {
        QStringList checked = getCheckedItems(model);
        AddTriggerFilter(true,checked);
    });

    QStringList checked = getCheckedItems(model);
    AddTriggerFilter(true,(select)?checked:QStringList());
}

void cardlist::ResetTypeComboBox(bool select){
    QListView *listView = new QListView(this->ui->typeComboBox);
    this->ui->typeComboBox->setView(listView);

    QStandardItemModel *model = new QStandardItemModel(this->ui->typeComboBox);
    this->ui->typeComboBox->setModel(model);
    for (int i = 0; i < this->available_type_filters.size(); i++)
    {
        QStandardItem *item = new QStandardItem(QString("").fromStdString(GetCardTypeString(this->available_type_filters[i])));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData((select)?Qt::Checked:Qt::Unchecked, Qt::CheckStateRole);
        model->appendRow(item);
    }

    connect(model, &QStandardItemModel::itemChanged, this, [=]() {
        QStringList checked = getCheckedItems(model);
        AddTypeFilter(true,checked);
    });

    QStringList checked = getCheckedItems(model);
    AddTypeFilter(true,(select)?checked:QStringList());
}

void cardlist::ResetColorComboBox(bool select){
    QListView *listView = new QListView(this->ui->colorComboBox);
    this->ui->colorComboBox->setView(listView);

    QStandardItemModel *model = new QStandardItemModel(this->ui->colorComboBox);
    this->ui->colorComboBox->setModel(model);
    for (int i = 0; i < this->available_color_filters.size(); i++)
    {
        QStandardItem *item = new QStandardItem(QString("").fromStdString(GetColorString(this->available_color_filters[i])));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData((select)?Qt::Checked:Qt::Unchecked, Qt::CheckStateRole);
        model->appendRow(item);
    }

    connect(model, &QStandardItemModel::itemChanged, this, [=]() {
        QStringList checked = getCheckedItems(model);
        AddColorFilter(true,checked);
    });

    QStringList checked = getCheckedItems(model);
    AddColorFilter(true,(select)?checked:QStringList());
}

void cardlist::ResetLevelComboBox(bool select){
    QListView *listView = new QListView(this->ui->levelComboBox);
    this->ui->levelComboBox->setView(listView);

    QStandardItemModel *model = new QStandardItemModel(this->ui->levelComboBox);
    this->ui->levelComboBox->setModel(model);
    for (int i = 0; i < this->available_level_filters.size(); i++)
    {
        QStandardItem *item = new QStandardItem(QString("").fromStdString(std::to_string(this->available_level_filters[i])));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData((select)?Qt::Checked:Qt::Unchecked, Qt::CheckStateRole);
        model->appendRow(item);
    }

    connect(model, &QStandardItemModel::itemChanged, this, [=]() {
        QStringList checked = getCheckedItems(model);
        AddLevelFilter(true,checked);
    });

    QStringList checked = getCheckedItems(model);
    AddLevelFilter(true,(select)?checked:QStringList());
}

void cardlist::ResetCostComboBox(bool select){
    QListView *listView = new QListView(this->ui->costComboBox);
    this->ui->costComboBox->setView(listView);

    QStandardItemModel *model = new QStandardItemModel(this->ui->costComboBox);
    this->ui->costComboBox->setModel(model);
    for (int i = 0; i < this->available_cost_filters.size(); i++)
    {
        QStandardItem *item = new QStandardItem(QString("").fromStdString(std::to_string(this->available_cost_filters[i])));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData((select)?Qt::Checked:Qt::Unchecked, Qt::CheckStateRole);
        model->appendRow(item);
    }

    connect(model, &QStandardItemModel::itemChanged, this, [=]() {
        QStringList checked = getCheckedItems(model);
        AddCostFilter(true,checked);
    });

    QStringList checked = getCheckedItems(model);
    AddCostFilter(true,(select)?checked:QStringList());
}

void cardlist::ResetTraitComboBox(bool select){
    QListView *listView = new QListView(this->ui->traitComboBox);
    this->ui->traitComboBox->setView(listView);

    QStandardItemModel *model = new QStandardItemModel(this->ui->traitComboBox);
    this->ui->traitComboBox->setModel(model);
    for (int i = 0; i < this->available_trait_filters.size(); i++)
    {
        QStandardItem *item = new QStandardItem(QString("").fromStdString(this->available_trait_filters[i]));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData((select)?Qt::Checked:Qt::Unchecked, Qt::CheckStateRole);
        model->appendRow(item);
    }

    connect(model, &QStandardItemModel::itemChanged, this, [=]() {
        QStringList checked = getCheckedItems(model);
        AddTraitFilter(true,checked);
    });

    QStringList checked = getCheckedItems(model);
    AddTraitFilter(true,(select)?checked:QStringList());
}

void cardlist::FillFiltersUsingSet(){
    if (this->choosen_sets.size() == 0){
        return;
    }



    this->available_level_filters = {}; // This will be loaded when laoding a set cards
    this->available_cost_filters = {};
    this->available_trigger_filters = {};
    this->current_trigger_filters = {};
    this->available_color_filters = {};
    this->current_level_filters = {};
    this->current_cost_filters = {};
    this->current_color_filters = {};
    this->available_type_filters = {};
    this->current_type_filters = {};

    this->available_trait_filters = {};
    this->current_trait_filters = {};

    for (int i {0}; i < this->choosen_sets.size(); ++i){

        std::map<std::string,Card*>::iterator it;
        for (it = this->choosen_sets.at(i)->getCards()->begin(); it != this->choosen_sets.at(i)->getCards()->end(); it++){
            std::string code = (*it).first;
            Card* c  = (*it).second;
            if(std::find(this->available_level_filters.begin(), this->available_level_filters.end(), c->getLevel()) == this->available_level_filters.end()) {
                this->available_level_filters.push_back(c->getLevel());
                this->current_level_filters.push_back(c->getLevel());
            }

            for (Trigger t : c->getTriggers()){
                if(std::find(this->available_trigger_filters.begin(), this->available_trigger_filters.end(), t) == this->available_trigger_filters.end()) {
                    this->available_trigger_filters.push_back(t);
                    this->current_trigger_filters.push_back(t);
                }
            }

            if(std::find(this->available_color_filters.begin(), this->available_color_filters.end(), c->getColor()) == this->available_color_filters.end()) {
                this->available_color_filters.push_back(c->getColor());
                this->current_color_filters.push_back(c->getColor());
            }

            if(std::find(this->available_cost_filters.begin(), this->available_cost_filters.end(), c->getCost()) == this->available_cost_filters.end()) {
                this->available_cost_filters.push_back(c->getCost());
                this->current_cost_filters.push_back(c->getCost());
            }

            if(std::find(this->available_type_filters.begin(), this->available_type_filters.end(), c->getCardType()) == this->available_type_filters.end()) {
                this->available_type_filters.push_back(c->getCardType());
                this->current_type_filters.push_back(c->getCardType());
            }
            for (std::string trait : c->getTraits()){
                if(std::find(this->available_trait_filters.begin(), this->available_trait_filters.end(), trait) == this->available_trait_filters.end()) {
                    this->available_trait_filters.push_back(trait);
                    this->current_trait_filters.push_back(trait);
                }
            }
            if (c->getColor() == Color::NONE){
                c->print();
                return;
            }
        }
    }

    for (int i {0} ; i < this->available_trigger_filters.size() ; ++i){
        std::cout << GetTriggerString(this->available_trigger_filters.at(i)) << std::endl;
    }

    std::cout << " -------" << std::endl;
    for (int i {0} ; i < this->current_trigger_filters.size() ; ++i){
        std::cout << GetTriggerString(this->current_trigger_filters.at(i)) << std::endl;
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
    std::sort(this->available_trigger_filters.begin(),this->available_trigger_filters.end());
    std::sort(this->current_trigger_filters.begin(),this->current_trigger_filters.end());

    this->ResetLevelComboBox();
    this->ResetTraitComboBox();
    this->ResetTriggerComboBox();
    this->ResetColorComboBox();
    this->ResetCostComboBox();
    this->ResetTypeComboBox();
}


void cardlist::FillCards(){
    this->all_cards_available = {};
    for (int i {0}; i < this->choosen_sets.size(); ++i){
        std::map<std::string,Card*>::iterator it;
        for (it = this->choosen_sets.at(i)->getCards()->begin(); it != this->choosen_sets.at(i)->getCards()->end(); it++){
            std::string code = (*it).first;
            Card* c  = (*it).second;
            this->all_cards_available.push_back(c);
        }
    }
}


void cardlist::ApplyFilter(){
    this->current_cards_to_display = {};
    std::string card_to_print = DataLoader::GetInstance()->GetCardToPrint();
    for (Card* c : this->all_cards_available){
        // apply filter by not adding filtered cards
        if (this->ui->searchFilterEdit->toPlainText() != ""){
            if (findSubstringIgnoreCase(this->ui->searchFilterEdit->toPlainText().toStdString(),c->getName())){
                this->current_cards_to_display.push_back(c);
            }
            else if (findSubstringIgnoreCase(this->ui->searchFilterEdit->toPlainText().toStdString(),c->getKey()))
            {
                this->current_cards_to_display.push_back(c);
            }
            else if (findSubstringIgnoreCase(this->ui->searchFilterEdit->toPlainText().toStdString(),c->getText()))
            {
                this->current_cards_to_display.push_back(c);
            }
        }
        else if (std::find(this->current_level_filters.begin(), this->current_level_filters.end(), c->getLevel()) != this->current_level_filters.end() &&
                 std::find(this->current_cost_filters.begin(), this->current_cost_filters.end(), c->getCost()) != this->current_cost_filters.end() &&
                 std::find(this->current_color_filters.begin(), this->current_color_filters.end(), c->getColor()) != this->current_color_filters.end() &&
                 std::find(this->current_type_filters.begin(), this->current_type_filters.end(), c->getCardType()) != this->current_type_filters.end())
        {
            bool include_due_to_traits = false;
            if (this->current_trait_filters.size() == this->available_trait_filters.size()){
                include_due_to_traits = true;
            }
            else {
                for (std::string trait : c->getTraits()){
                    if (std::find(this->current_trait_filters.begin(), this->current_trait_filters.end(), trait) != this->current_trait_filters.end()){
                        include_due_to_traits = true;
                    }
                }
            }

            bool include_due_to_triggers = false;
            if (this->current_trigger_filters.size() == this->available_trigger_filters.size()){
                include_due_to_triggers = true;
            }
            else if (this->current_trigger_filters.size() == 0 && c->getTriggers().size() == 0) {
                include_due_to_triggers = true;
            }
            else {
                for (Trigger t : c->getTriggers()){
                    if (std::find(this->current_trigger_filters.begin(), this->current_trigger_filters.end(), t) != this->current_trigger_filters.end()){
                        include_due_to_triggers = true;
                    }
                }
            }


            if ((include_due_to_traits || this->available_trait_filters.size() == 0) && (include_due_to_triggers || this->available_trigger_filters.size() == 0)){
                this->current_cards_to_display.push_back(c);
            }
        }
    }
}

void cardlist::OnSeriePick(){
    this->UnloadData(false);
    std::string choosen_serie_name = this->ui->seriePickBox->currentText().toStdString();
    //QMessageBox::information(this, "Item Selection",
    //                         this->m_series_dropdown->currentText());

    this->picked_serie = true;
    this->picked_sets = false;
    this->UpdateUi();
    this->ui->setPickBox->clear();
    std::vector<Serie*>* series = DataLoader::GetInstance()->getSeries();
    for (int i {0}; i < series->size(); ++i){
        if (series->at(i)->getName() ==  choosen_serie_name){
            this->choosen_serie = series->at(i);
        }

    }

    if (this->choosen_serie != nullptr){
        for (Set* se : *(this->choosen_serie->getAllSets())){
            this->ui->setPickBox->addItem(QString::fromStdString(se->getName()));
        }
    }
    else{
        std::cout << "Error, serie not found among the list : " << this->ui->seriePickBox->currentText().toStdString() << std::endl;
    }

}



void cardlist::OnSetPick(){
    this->UnloadData(false);

    std::string choosen_set_name = this->ui->setPickBox->currentText().toStdString();
    this->picked_serie = true;
    this->picked_sets = true;
    this->UpdateUi();
    this->choosen_sets.clear();
    if (this->choosen_serie != nullptr){
        for (int i {0}; i < this->choosen_serie->getAllSets()->size(); ++i){
            if (this->choosen_serie->getAllSets()->at(i)->getName() ==  choosen_set_name){
                this->choosen_sets.push_back(this->choosen_serie->getAllSets()->at(i));
            }

        }
    }
    this->FillFiltersUsingSet();
    this->FillCards();
    this->ApplyFilter();
    SortFilteredCards(&this->current_cards_to_display,this->current_orders);
    this->DisplayFilteredCards();


}

void cardlist::OnSerieCardsPick(){
    this->UnloadData(false);
    std::string choosen_serie_name = this->ui->seriePickBox->currentText().toStdString();
    //QMessageBox::information(this, "Item Selection",
    //                         this->m_series_dropdown->currentText());

    this->picked_serie = true;
    this->picked_sets = true;
    this->UpdateUi();
    this->ui->setPickBox->clear();
    this->choosen_sets.clear();
    std::vector<Serie*>* series = DataLoader::GetInstance()->getSeries();
    for (int i {0}; i < series->size(); ++i){
        if (series->at(i)->getName() ==  choosen_serie_name){
            this->choosen_serie = series->at(i);
        }

    }

    if (this->choosen_serie != nullptr){
        for (Set* se : *(this->choosen_serie->getAllSets())){
            this->ui->setPickBox->addItem(QString::fromStdString(se->getName()));
        }
    }
    else{
        std::cout << "Error, serie not found among the list : " << this->ui->seriePickBox->currentText().toStdString() << std::endl;
    }

    if (this->choosen_serie != nullptr){
        for (int i {0}; i < this->choosen_serie->getAllSets()->size(); ++i){
            this->choosen_sets.push_back(this->choosen_serie->getAllSets()->at(i));
        }
    }

    this->FillFiltersUsingSet();
    this->FillCards();
    this->ApplyFilter();
    SortFilteredCards(&this->current_cards_to_display,this->current_orders);
    this->DisplayFilteredCards();
}


void cardlist::DisplayFilteredCards(){

    this->ui->cardGridWidget_2->setWordWrap(true);
    this->ui->cardGridWidget_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->ui->cardGridWidget_2->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    for (Card* c : this->current_cards_to_display){
        //std::cout << c.getImagePath() << std::endl;
        if (c->getImagePath() != "" ){
            std::string path_str = c->getImagePath();
            //path_str.replace(path_str.find(" "),std::string(" ").size(),"\ ");
            QString path_image = QString::fromStdString(path_str);
            QWidget* final_widget = new QWidget();
            QHBoxLayout* qblayout = new QHBoxLayout(final_widget);
            qblayout->setContentsMargins(0,0,0,0);
            if (QFile::exists(path_image)) {
                QPixmap pix(path_image);
                QLabel* textLabel = new QLabel(QString::fromStdString(c->getCardHTML()));
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
