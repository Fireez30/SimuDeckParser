#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils.h"
#include<iostream>
#include<QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <filesystem>
namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //data
    this->series = {};
    this->choosen_serie = nullptr;
    this->choosen_set = nullptr;

    //display state
    this->display_load_series = true;
    this->display_pick_set = false;
    this->display_cards = false;
    // layouts setup
    this->m_simu_path_field = new QLineEdit(this);
    this->m_simu_path_field->setGeometry(10,10,600,30);
    this->m_button_loadseries = new QPushButton("Load series",this);
    this->m_button_loadseries->setGeometry(620,10,80,30);
    this->m_series_dropdown = new QComboBox(this);
    this->m_series_dropdown->setPlaceholderText(QString::fromStdString("Pick a serie"));
    this->m_series_dropdown->setGeometry(10,10,400,30);
    this->m_series_pick_button = new QPushButton("Validate",this);
    this->m_series_pick_button->setGeometry(420,10,80,30);

    this->m_set_dropdown = new QComboBox(this);
    this->m_set_dropdown->setPlaceholderText(QString::fromStdString("Pick a set"));
    this->m_set_dropdown->setGeometry(10,50,500,30);
    this->m_set_pick_button = new QPushButton("Validate",this);
    this->m_set_pick_button->setGeometry(520,50,80,30);

    this->UpdateUi();
    //binding signals
    connect(this->m_button_loadseries, SIGNAL (clicked(bool)), this, SLOT (LoadButtonClicked()));
    connect(this->m_series_pick_button, SIGNAL (clicked(bool)), this, SLOT (OnSeriePick()));
    connect(this->m_set_pick_button, SIGNAL (clicked(bool)), this, SLOT (OnSetPick()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateUi(){
    this->m_button_loadseries->setVisible(this->display_load_series);
    this->m_simu_path_field->setVisible(this->display_load_series);
    this->m_series_dropdown->setVisible(!this->display_load_series);
    this->m_series_pick_button->setVisible(!this->display_load_series);
    this->m_set_pick_button->setVisible(this->display_pick_set);
    this->m_set_dropdown->setVisible(this->display_pick_set);

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
    std::string choosen_serie_name = this->m_series_dropdown->currentText().toStdString();
    //QMessageBox::information(this, "Item Selection",
    //                         this->m_series_dropdown->currentText());
    this->display_pick_set = true;
    this->UpdateUi();
    this->m_set_dropdown->clear();
    for (int i {0}; i < this->series.size(); ++i){
        if (this->series.at(i).getName() ==  choosen_serie_name){
            this->choosen_serie = &this->series.at(i);
        }

    }

    if (this->choosen_serie != nullptr){
        for (Set se : this->choosen_serie->getAllSets()){
            this->m_set_dropdown->addItem(QString::fromStdString(se.getName()));
        }
    }
    else{
        std::cout << "Error, serie not found among the list : " << this->m_series_dropdown->currentText().toStdString() << std::endl;
    }

}

void MainWindow::OnSetPick(){
    std::string choosen_set_name = this->m_set_dropdown->currentText().toStdString();
    this->display_cards = true;
    this->UpdateUi();
    if (this->choosen_serie != nullptr){
        for (int i {0}; i < this->choosen_serie->getAllSets().size(); ++i){
            if (this->choosen_serie->getAllSets().at(i).getName() ==  choosen_set_name){
                this->choosen_set = &this->choosen_serie->getAllSets().at(i);
            }

        }
    }
}

void MainWindow::LoadButtonClicked(){
    //"/home/ben/Games/Weiss Schwarz 0.6.3.2 Linux/Weiss Schwarz 0.6.3.2 Linux_Data/"
    std::string simulator_path { this->m_simu_path_field->text().toStdString() };
    if (fs::exists(simulator_path)){
        // internal subfolders
        std::string cards_folder = simulator_path+"StreamingAssets"+separator()+"Cards"+separator();
        // parsing all series
        this->series = ParseSeries(cards_folder);


        for (Serie sa : this->series){
            std::cout << sa.getName() << " : " << sa.getAllSets().size() << std::endl;
            for (Set set_obj : sa.getAllSets()){
                std::cout << set_obj.getName() << " , " << set_obj.getKey() << " : " << set_obj.getCards().size() << std::endl;
            }
            this->m_series_dropdown->addItem(QString::fromStdString(sa.getName()));
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
