#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<iostream>
#include<QLineEdit>
#include "utils.h"
#include <QComboBox>
#include <QFile>
#include <QLabel>
#include <QScrollArea>
#include <QMessageBox>
#include <filesystem>
namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent,int grid_width,int grid_height)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //data
    this->series = {};
    this->currentCardsImages={};
    this->current_cards_to_display = {};
    this->choosen_serie = nullptr;
    this->choosen_set = nullptr;
    this->grid_height = grid_height;
    this->grid_width = grid_width;
    this->current_cards_index = 0;


    this->ui->cardGridWidget_2->setViewMode(QListView::ListMode);
    this->ui->cardGridWidget_2->setResizeMode(QListView::Adjust);
    this->ui->cardGridWidget_2->setIconSize(QSize(280, 391));
    //orders and filters
    this->current_orders = {Orders::LEVEL_ASCENDING,Orders::POWER_ASCENDING}; // Order with the first, if equals, order with the next, etc ...

    this->available_level_filters = {}; // This will be loaded when laoding a set cards
    this->available_cost_filters = {};
    this->available_color_filters = {};
    this->current_level_filters = {};
    this->current_cost_filters = {};
    this->current_color_filters = {};
    this->available_type_filters = {};
    this->current_type_filters = {};
    //display state
    this->display_load_series = true;
    this->display_pick_set = false;
    this->display_cards = false;
    // layouts setup


    this->UpdateUi();
    //binding signals
    connect(this->ui->loadSimulatorButton, SIGNAL (clicked(bool)), this, SLOT (LoadButtonClicked()));
    connect(this->ui->serieLoadButton, SIGNAL (clicked(bool)), this, SLOT (OnSeriePick()));
    connect(this->ui->setLoadButton, SIGNAL (clicked(bool)), this, SLOT (OnSetPick()));
    connect(this->ui->actionExit, SIGNAL (triggered()), this, SLOT (OnExit()));
    connect(this->ui->actionUnload_simulator, SIGNAL (triggered()), this,SLOT (OnUnloadSimulator()));
    connect(this->ui->testFiltersButton,SIGNAL (clicked(bool)), this, SLOT (TestFiltersAndSorts()));
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

void MainWindow::TestFiltersAndSorts(){
    std::cout << "test filter and sorts" << std::endl;
    WriteCardsToFile(this->current_cards_to_display,"/home/ben/cards_to_display_before.txt");
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
    this->FillCardsUsingFilters();
    this->SortFilteredCards();

    std::cout << "Found " << this->current_cards_to_display.size() << " red only card cards " << std::endl;
    for (Card c2 : this->current_cards_to_display){
        std::cout << c2.getName() << std::endl;
        std::cout << "Card level " << c2.getLevel() << " color " << GetColorString(c2.getColor()) << std::endl;
    }
    WriteCardsToFile(this->current_cards_to_display,"/home/ben/cards_to_display_after.txt");
}
void MainWindow::FillFiltersUsingSet(){
    if (this->choosen_set == nullptr){
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

    std::vector<Card>& cards = this->choosen_set->getCards();
    for (Card c : cards){
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
        if (c.getColor() == Color::NONE){
            c.print();
            return;
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

    std::cout << "Available level filters"  << std::endl;
    for (int l : this->available_level_filters){
        std::cout << l << " , ";
    }
    std::cout << std::endl;

    std::cout << "Current level filters"  << std::endl;
    for (int l : this->current_level_filters){
        std::cout << l << " , ";
    }
    std::cout << std::endl;


    std::cout << "Available color filters"  << std::endl;
    for (Color l : this->available_color_filters){
        std::cout << GetColorString(l) << " , ";
    }
    std::cout << std::endl;

    std::cout << "Current color filters"  << std::endl;
    for (Color l : this->current_color_filters){
        std::cout <<  GetColorString(l) << " , ";
    }
    std::cout << std::endl;


    std::cout << "Available cost filters"  << std::endl;
    for (int l : this->available_cost_filters){
        std::cout << l << " , ";
    }
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
void MainWindow::UpdateUi(){
    //this->display_load_series = true;
    //this->display_pick_set = false;
    //this->display_cards = false;
    this->ui->simulatorWidget->setVisible(this->display_load_series);
    this->ui->serieAndSetWidget->setVisible(!this->display_load_series);
    this->ui->serieWidget_2->setVisible(!this->display_load_series);
    this->ui->setWidget_2->setVisible(this->display_pick_set);
    this->ui->CardWholeAreaWidget_2->setVisible(this->display_pick_set);
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

void MainWindow::FillCardsUsingFilters(){
    this->current_cards_to_display = {};
    std::vector<Card>& cards = this->choosen_set->getCards();
    for (Card c : cards){
        // apply filter by not adding filtered cards
        if (std::find(this->current_level_filters.begin(), this->current_level_filters.end(), c.getLevel()) != this->current_level_filters.end() &&
            std::find(this->current_cost_filters.begin(), this->current_cost_filters.end(), c.getCost()) != this->current_cost_filters.end() &&
            std::find(this->current_color_filters.begin(), this->current_color_filters.end(), c.getColor()) != this->current_color_filters.end() &&
            std::find(this->current_type_filters.begin(), this->current_type_filters.end(), c.getCardType()) != this->current_type_filters.end())
        {
            this->current_cards_to_display.push_back(c);
        }
    }
}
void MainWindow::SortFilteredCards(){
    std::sort(this->current_cards_to_display.begin(), this->current_cards_to_display.end(), [this](Card a, Card b) { // lambda func
        if (this->current_orders.size()){
            for (Orders o : this->current_orders){
                if (a.getColor() != b.getColor()){
                    return a.getColor() < b.getColor();
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
    std::string choosen_set_name = this->ui->setPickBox->currentText().toStdString();
    this->display_cards = true;
    this->UpdateUi();
    if (this->choosen_serie != nullptr){
        for (int i {0}; i < this->choosen_serie->getAllSets().size(); ++i){
            if (this->choosen_serie->getAllSets().at(i).getName() ==  choosen_set_name){
                this->choosen_set = &this->choosen_serie->getAllSets().at(i);
            }

        }
    }

    this->FillFiltersUsingSet();
    this->FillCardsUsingFilters();
    this->SortFilteredCards();
    this->DisplayFilteredCards();
    std::cout << "Found " << this->current_cards_to_display.size() << " cards " << std::endl;


}

void MainWindow::DisplayFilteredCards(){
    int row = 0, col = 0;
    const int columns = 4; // adjust grid width

    for (Card c : this->current_cards_to_display){
        /*

        QLabel label = QLabel(this->ui->cardGridWidget_2);
        QPixmap pixmap(QString::fromStdString(c.getImagePath()));
        label.setPixmap(pixmap);
        label.setMask(pixmap.mask());
        label.setFixedWidth(30);
        label.setFixedHeight(40);
        label.setVisible(true);
        this->currentCardsImages.push_back(&label);
        std::cout << "added label" << std::endl;
*/
        std::cout << c.getImagePath() << std::endl;
        if (c.getImagePath() != "" ){
            std::string path_str = c.getImagePath();
            //path_str.replace(path_str.find(" "),std::string(" ").size(),"\ ");
            QString path_image = QString::fromStdString(path_str);
            QWidget* final_widget = new QWidget();
            QHBoxLayout* qblayout = new QHBoxLayout(final_widget);
            if (QFile::exists(path_image)) {
                QPixmap pix(path_image);
                QLabel* textLabel = new QLabel("<p><b>This text is bold</b></p>"
"<p><i>This text is italic</i></p>"
"<p>This is<sub> subscript</sub> and <sup>superscript</sup></p>");
                QLabel* imgLabel = new QLabel();
                textLabel->setTextFormat(Qt::RichText);
                imgLabel->setScaledContents( true );
                imgLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
                imgLabel->setPixmap(pix);
                imgLabel->setFixedSize(280, 391); // add padding around thumbnail
                textLabel->setMinimumHeight(391);
                textLabel->setAlignment(Qt::AlignLeft);
                qblayout->addWidget(imgLabel);
                qblayout->addWidget(textLabel);
                //qblayout->addStretch(1);
                //qblayout->setSpacing(0);
                //imgLabel->setAlignment(Qt::AlignCenter);*/
                final_widget->setFixedHeight(391);
                QListWidgetItem *item = new QListWidgetItem();
                this->ui->cardGridWidget_2->addItem(item);
                this->ui->cardGridWidget_2->setItemWidget(item,final_widget);
                item->setSizeHint( final_widget->sizeHint() );
                //this->ui->cardGridWidget_2->addWidget(imgLabel);

                if (++col >= columns) {
                    col = 0;
                    ++row;
                }
            } else {
                qWarning() << "Image not found:" << path_image;
            }

        }

    }
    //this->ui->cardGridWidget_2->setFixedSize(ui->cardGridWidget_2->sizeHintForColumn(0) + ui->cardGridWidget_2->frameWidth() * 2,
    //                             ui->cardGridWidget_2->sizeHintForRow(0) * ui->cardGridWidget_2->count() + 2 * ui->cardGridWidget_2->frameWidth());

}

void MainWindow::DestroyDisplayedCards(){

}
void MainWindow::OnExit(){
    QApplication::quit();
}

void MainWindow::OnUnloadSimulator(){
    this->UnloadData();
}

void MainWindow::UnloadData(){
    this->available_level_filters.clear(); // Clear memory of objects in the vector
    this->available_level_filters = {}; // completely reset the size of the vector itself
    this->available_cost_filters.clear(); // Clear memory of objects in the vector
    this->available_cost_filters = {}; // completely reset the size of the vector itself
    this->available_color_filters.clear(); // Clear memory of objects in the vector
    this->available_color_filters = {}; // completely reset the size of the vector itself
    this->available_type_filters.clear(); // Clear memory of objects in the vector
    this->available_type_filters = {}; // completely reset the size of the vector itself


    this->current_level_filters.clear(); // Clear memory of objects in the vector
    this->current_level_filters = {}; // completely reset the size of the vector itself
    this->current_cost_filters.clear(); // Clear memory of objects in the vector
    this->current_cost_filters = {}; // completely reset the size of the vector itself
    this->current_type_filters.clear(); // Clear memory of objects in the vector
    this->current_type_filters = {}; // completely reset the size of the vector itself
    this->current_color_filters.clear(); // Clear memory of objects in the vector
    this->current_color_filters = {}; // completely reset the size of the vector itself

    this->current_orders.clear();
    this->current_orders = {Orders::LEVEL_ASCENDING,Orders::POWER_ASCENDING};

    this->series.clear();
    this->series = {};

    this->choosen_serie = nullptr;
    this->choosen_set = nullptr;

    this->display_load_series = true;
    this->display_pick_set = false;
    this->display_cards = false;

    this->ui->simulatorPathBox->clear();
    this->ui->seriePickBox->clear();
    this->ui->setPickBox->clear();

    this->UpdateUi();

}

void MainWindow::LoadButtonClicked(){
    //"/home/ben/Games/Weiss Schwarz 0.6.3.2 Linux/Weiss Schwarz 0.6.3.2 Linux_Data/"
    std::string simulator_path { this->ui->simulatorPathBox->text().toStdString() };
    if (fs::exists(simulator_path)){
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
        this->UpdateUi();
    }
    else
    {
        QMessageBox::information(this, "Path chosen",
                                 "Simulator path is incorrect, verify and try again");
    }
}
