#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<iostream>
#include<QLineEdit>
#include "utils.h"
#include <QComboBox>
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
    this->choosen_serie = nullptr;
    this->choosen_set = nullptr;
    this->grid_height = grid_height;
    this->grid_width = grid_width;
    this->current_cards_index = 0;

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
}

MainWindow::~MainWindow()
{
    delete ui;
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
    this->ui->setWidget_2->setVisible(display_pick_set);
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
