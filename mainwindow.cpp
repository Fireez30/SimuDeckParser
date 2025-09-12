#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils.h"
#include<iostream>
#include<QLineEdit>
#include <filesystem>
namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //data
    this->series = {};

    //display state
    this->display_load_series = true;

    // layouts setup
    this->m_simu_path_field = new QLineEdit(this);
    this->m_simu_path_field->setGeometry(10,10,600,30);
    this->m_button_loadseries = new QPushButton("Load series",this);
    this->m_button_loadseries->setGeometry(620,10,80,30);

    //binding signals
    connect(this->m_button_loadseries, SIGNAL (clicked(bool)), this, SLOT (LoadButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateUi(){
    this->m_button_loadseries->setVisible(this->display_load_series);
    this->m_simu_path_field->setVisible(this->display_load_series);
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
        }
        this->display_load_series = false;
        this->UpdateUi();
    }
    else
    {
        std::cout << "Simulator path is incorrect, verify and try again" << std::endl;
    }
}
