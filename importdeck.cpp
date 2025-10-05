#include "importdeck.h"
#include "ui_importdeck.h"

importdeck::importdeck(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::importdeck)
{
    ui->setupUi(this);
}

importdeck::~importdeck()
{
    delete ui;
}
