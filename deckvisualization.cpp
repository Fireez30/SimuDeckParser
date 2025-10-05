#include "deckvisualization.h"
#include "ui_deckvisualization.h"
#include "dataloader.h"
#include "hoverlabel.h"
#include "algorithms.h"
#include <QUiLoader>
#include <QFile>
deckvisualization::deckvisualization(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::deckvisualization) , parent(parent)
{
    ui->setupUi(this);
    this->picked_deck = "";
    this->ui->cardImage->setScaledContents( true );
    this->ui->cardImage->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    this->ui->cardImage->clear();
    this->UnlockCardPanel();

    this->ui->cardText->clear();
    this->ui->cardText->setWordWrap(true);
    //textLabel->setIndent(224);
    this->ui->cardText->setTextFormat(Qt::RichText);
    this->ui->cardText->setMaximumWidth(1300);
    this->ui->cardText->setAlignment(Qt::AlignLeft);

    this->ui->deckPixBox->clear();
    std::map<std::string,Deck*>* decks = DataLoader::GetInstance()->getDecks();
    std::map<std::string,Deck*>::iterator it;
    for (it = decks->begin(); it != decks->end(); it++){
        std::string deck_key = (*it).first;
        Deck *deck = (*it).second;
        this->ui->deckPixBox->addItem(QString::fromStdString(deck->getName()));
    }

    connect(this->ui->backButton_2, SIGNAL (clicked(bool)),this, SLOT (SwitchToMainMenu()));
    connect(this->ui->pickDeckButton, SIGNAL (clicked(bool)),this, SLOT (PickDeck()));

}


deckvisualization::~deckvisualization()
{
    delete ui;
}

void deckvisualization::SwitchToMainMenu(){
    this->ClearDeckWidget();
    this->switchToUi(QString(":/forms/mainwindow.ui"));
}

void deckvisualization::ClearDeckWidget(){

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



void deckvisualization::switchToUi(const QString &uiFilePath)
{
    QUiLoader loader;
    QFile file(uiFilePath);
    if (file.open(QFile::ReadOnly)) {
        QWidget *newWidget = loader.load(&file, this);
        file.close();
        parent.setCentralWidget(newWidget);
    }
}


void deckvisualization::DisplayPickedDeck(){
    //std::cout << "Display picked : " << this->picked_deck << std::endl;
    int row = 0, col = 0;
    const int columns = 10;
    Deck* de = DataLoader::GetInstance()->getDeck(this->picked_deck);
    if (de != nullptr){
        std::vector<Orders> deck_orders = {Orders::TYPE_ASCENDING,Orders::LEVEL_ASCENDING,Orders::COST_ASCENDING,Orders::POWER_ASCENDING,Orders::COLOR};
        std::vector<Card> card_list = de->getCardList();
        SortFilteredCards(&card_list,deck_orders);
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

void deckvisualization::ShowCardToSidePanel(Card c){
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

void deckvisualization::UnlockCardPanel(){
    this->lock_card_panel = false;
    this->ui->cardImage->setStyleSheet(QString(""));

}

void deckvisualization::LockCardPanel(Card c){
    this->lock_card_panel = false;
    this->ShowCardToSidePanel(c);
    this->lock_card_panel = true;
    this->ui->cardImage->setStyleSheet(QString("border-style:solid;border-width:2px;border-color:red"));
}

void deckvisualization::PickDeck(){
    this->ClearDeckWidget();
    std::string deck_name = this->ui->deckPixBox->currentText().toStdString();
    //LoadDeck(this->decks,deck_name,this->series);
    this->picked_deck = deck_name;
    Deck* deck = DataLoader::GetInstance()->getDeck(deck_name);
    if (deck != nullptr){
        deck->Print();
        if (deck->getCardList().size() == 0){
            DataLoader::GetInstance()->LoadDeck(deck_name);
        }
    }
    else {
        DataLoader::GetInstance()->LoadDeck(deck_name);
    }
    this->UnlockCardPanel();
    this->DisplayPickedDeck();
}
