#include "cardlist.h"
#include "ui_cardlist.h"
#include "dataloader.h"
#include <QUiLoader>
#include <QFile>
#include <iostream>
#include <QCheckBox>
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

    this->display_load_series = true;
    this->display_pick_set = false;
    this->display_cards = false;

    this->currentCardsImages={};
    this->current_cards_to_display = {};

    this->cards_items = {};
    this->available_level_filters = {}; // This will be loaded when laoding a set cards
    this->available_cost_filters = {};
    this->available_color_filters = {};
    this->available_trait_filters = {};
    this->available_type_filters = {};
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

    //display state
    this->display_load_series = true;
    this->display_pick_set = false;
    this->display_cards = false;
}


cardlist::~cardlist()
{
    delete ui;
}

void cardlist::UpdateUi(){
    this->ui->serieAndSetWidget->setVisible(true);
    this->ui->serieWidget_2->setVisible(true);
    this->ui->setWidget_2->setVisible(this->display_pick_set);
    this->ui->CardWholeAreaWidget_2->setVisible(this->display_pick_set);
    this->ui->searchWidget->setVisible(this->display_pick_set);
    this->ui->setLoadButton->setVisible(this->display_pick_set);

    this->ui->seriePickBox->setVisible(true);
    this->ui->setPickBox->setVisible(true);
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
    if (clear_filters){
        clearLayout(this->ui->costFilterLayout_2);
        clearLayout(this->ui->colorFilterLayout_2);
        clearLayout(this->ui->typeFilterLayout);
        clearLayout(this->ui->traitFilterLayout);
        clearLayout(this->ui->levelFilterLayout_2);
    }


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
        this->display_load_series = true;
        this->display_pick_set = false;
        this->display_cards = false;

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


void cardlist::AddCostFilter(bool active,int level){
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

void cardlist::AddLevelFilter(bool active,int level){
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

void cardlist::AddTypeFilter(bool active, CardType type){
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
void cardlist::AddColorFilter(bool active,Color color){
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

void cardlist::AddTraitFilter(bool active,std::string trait){
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

void cardlist::FillFiltersUsingSet(){
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


void cardlist::FillCards(){
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


void cardlist::ApplyFilter(){
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

void cardlist::OnSeriePick(){
    this->UnloadData(false);
    std::string choosen_serie_name = this->ui->seriePickBox->currentText().toStdString();
    //QMessageBox::information(this, "Item Selection",
    //                         this->m_series_dropdown->currentText());
    this->display_pick_set = true;
    this->UpdateUi();
    this->ui->setPickBox->clear();
    std::vector<Serie*>* series = DataLoader::GetInstance()->getSeries();
    for (int i {0}; i < series->size(); ++i){
        if (series->at(i)->getName() ==  choosen_serie_name){
            this->choosen_serie = series->at(i);
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



void cardlist::OnSetPick(){
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
    SortFilteredCards(&this->current_cards_to_display,this->current_orders);
    this->DisplayFilteredCards();


}

void cardlist::OnSerieCardsPick(){
    this->UnloadData(false);
    std::string choosen_serie_name = this->ui->seriePickBox->currentText().toStdString();
    //QMessageBox::information(this, "Item Selection",
    //                         this->m_series_dropdown->currentText());
    this->display_pick_set = true;
    this->display_cards = true;
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
    SortFilteredCards(&this->current_cards_to_display,this->current_orders);
    this->DisplayFilteredCards();
}


void cardlist::DisplayFilteredCards(){

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
