#include "cardlist.h"
#include "ui_cardlist.h"
#include "dataloader.h"

cardlist::cardlist(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::cardlist)
{
    ui->setupUi(this);
    std::vector<Serie*>* series = DataLoader::GetInstance()->getSeries();


    for (Serie* sa : *series){
        this->ui->seriePickBox->addItem(QString::fromStdString(sa->getName()));
    }

}

cardlist::~cardlist()
{
    delete ui;
}
