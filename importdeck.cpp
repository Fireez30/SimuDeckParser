#include "importdeck.h"
#include "ui_importdeck.h"
#include <QUiLoader>
#include <QFile>
#include "algorithms.h"
#include "mainwindow.h"
#include "filedownloader.h"
#include <QFont>
#include "dataloader.h"
#include <QMessageBox>
#include <iostream>
#include <QTimer>
importdeck::importdeck(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::importdeck)
{
    ui->setupUi(this);
    this->number_of_images_started = 0;
    this->number_of_images_downloaded = 0;
    this->image_cards = {};
    this->match_changed_cards = {};
    this->ui->scrollArea->setVisible(false);
    this->ui->cardComparisonWidget->setVisible(false);
    this->ui->startImport->setVisible(false);
    connect(this->ui->ImportButton, SIGNAL (clicked(bool)),this, SLOT (ImportLink()));
    connect(this->ui->closeImportPanelButton, SIGNAL(clicked(bool)),this,SLOT(SwitchToMainMenu()));
    connect(this->ui->startImport,SIGNAL (clicked(bool)),this, SLOT (AddToSimu()));
}

importdeck::~importdeck()
{
    delete ui;
}



void importdeck::ImportLink(){
    std::string link = this->ui->EncoreDeckLinkField->text().toStdString();
    std::vector<std::string> splitted_link  = split(link,'/');
    if (splitted_link.size() > 4){
        this->number_of_images_started = 0;
        this->number_of_images_downloaded = 0;
        ParsedDeck parsed = DataLoader::GetInstance()->ParseDeckById(splitted_link.back());
        this->deck_name = parsed.name;
        this->deck_date = parsed.date;
        this->deck_init_card_list = parsed.cards;
        this->deck_final_card_list = {};
        this->match_changed_cards = {};
        this->image_cards.clear();
        this->image_cards = {};
        this->ui->deckDateLabel->setText(QString().fromStdString("Creation date : "+this->deck_date));
        this->ui->deckNameLabel->setText(QString().fromStdString(this->deck_name));

        std::vector<Card*> final_cards = DataLoader::GetInstance()->AdaptDeckList(parsed.cards);

        if (final_cards.size() == 0 && this->deck_init_card_list.size() > 0){
            // QALERT
            QMessageBox dlg = QMessageBox(this);
            dlg.setWindowTitle("Error during deck import");
            dlg.setText("No cards found for this deck, are you sure the serie or the set is availabl in the simulator ?");
            dlg.exec();
            return;
        }

        if (final_cards.size() < this->deck_init_card_list.size()){
            //QALERT
            QMessageBox dlg = QMessageBox(this);
            dlg.setWindowTitle("Error during deck import");
            dlg.setText("Some of Encoredeck's cards could not be found in your simulator, please contact this tool developer.");
            dlg.exec();
            return;
        }

        if (final_cards.size() != 50 && this->deck_init_card_list.size() != 50){
            //QALERT
            QMessageBox dlg = QMessageBox(this);
            dlg.setWindowTitle("Error during deck import");
            dlg.setText("Both decks do not hold 50 cards, please verify your deck");
            dlg.exec();
            return;
        }

        for (Card* c : final_cards){
            if (c != nullptr){
                this->deck_final_card_list.push_back(c->getKey());
            }
        }
        std::string base_deck_to_display = "";
        //for (std::string card : this->deck_init_card_list){
        //    base_deck_to_display += card + "\n";
        //}

        std::string base_deck_to_display_new = "";
        //for (std::string card : this->deck_final_card_list){
        //    base_deck_to_display_new += card + "\n";
        //}
        int seconds_timeout_download = 1;
        for (int i {0}; i < this->deck_init_card_list.size(); ++i){
            if (trim(this->deck_init_card_list.at(i)) != trim(final_cards.at(i)->getKey())){
                //std::cout << this->deck_init_card_list.at(i) << " <-> " << final_cards.at(i)->getKey() << std::endl;
                if (final_cards.at(i) != nullptr && this->match_changed_cards.find(this->deck_init_card_list.at(i)) == this->match_changed_cards.end()){
                    std::string init_card_code = this->deck_init_card_list.at(i);
                    base_deck_to_display +=  init_card_code+"\n";
                    base_deck_to_display_new += final_cards.at(i)->getKey() + "\n";
                    std::string final_image_path = final_cards.at(i)->getImagePath();
                    std::string init_image_url = "https://www.encoredecks.com/images/";
                    std::string lang_folder = "JP";
                    std::string serie_code = "";
                    std::vector<std::string> split_card_codes = split(init_card_code,'-');
                    if (split_card_codes.size() > 1){
                        std::string cardnumber = split_card_codes[1];
                        if (cardnumber.find("E") != std::string::npos){
                            lang_folder = "EN";
                        }
                        std::transform(cardnumber.begin(),cardnumber.end(),cardnumber.begin(),::toupper);
                        std::vector<std::string> split_deck_code = split(split_card_codes[0],'/');
                        if (split_deck_code.size() > 1){
                            std::string deck_code = split_deck_code[1];
                            std::transform(deck_code.begin(),deck_code.end(),deck_code.begin(),::toupper);
                            init_image_url += "/"+lang_folder+"/"+deck_code+"/"+cardnumber+".gif";
                        }

                        if (this->image_cards.find(init_card_code) == this->image_cards.end()){
                            // TODO
                            //std::cout<< "requesting image : " << init_image_url << std::endl;
                            ImageDownloader* downloader;

                            /*QUrl imageUrl(QString().fromStdString(init_image_url));
                            this->number_of_images_started += 1;
                            FileDownloader* m_pImgCtrl = new FileDownloader(imageUrl, this);*/
                            QPixmap* buttonImage  = new QPixmap();
                            QObject::connect(downloader, &ImageDownloader::imageDownloaded,[this,init_card_code,buttonImage](const QByteArray &image){
                                buttonImage->loadFromData(image);
                                this->OnImageDownloaded(init_card_code,buttonImage);});
                            QTimer::singleShot(1000, [downloader,init_image_url]() {
                                downloader->downloadImage(QUrl(QString().fromStdString(init_image_url)));
                            });

                            /*buttonImage->loadFromData(m_pImgCtrl->downloadedData());
                            connect(m_pImgCtrl, &FileDownloader::downloaded,this, [this,init_card_code,buttonImage]() {
                                this->OnImageDownloaded(init_card_code,buttonImage);
                            });*/

                        }

                        if (this->image_cards.find(final_cards.at(i)->getKey()) == this->image_cards.end()){
                            // Load file and add to map
                            QString path_image = QString::fromStdString(final_image_path);
                            this->image_cards.insert_or_assign(final_cards.at(i)->getKey(),new QPixmap(path_image));
                        }

                        if (this->match_changed_cards.find(init_card_code) == this->match_changed_cards.end()){

                            this->match_changed_cards.insert_or_assign(init_card_code,final_cards.at(i)->getKey());
                        }

                    }
                }

                // stored into : cardComparisonWidget
                // loop over map changed cards.
                // For each key, fill the view with loaded pixmap
            }
        }

    }
}

void importdeck::OnImageDownloaded(std::string init_card_code,QPixmap* buttonImage){
    this->image_cards.insert_or_assign(init_card_code,buttonImage);
    this->number_of_images_downloaded += 1;
    if (this->number_of_images_downloaded >= this->number_of_images_started && this->number_of_images_started > 0){
        std::cout << "found " << this->number_of_images_downloaded << std::endl;
        std::string to_display = "";
        if (this->match_changed_cards.size() == 0){
            to_display = "All cards found in Encore decks have been found in the simulator. \n the deck can be imported in the simulator";
        }

        if (this->match_changed_cards.size() > 0){
            to_display = "Some cards found in Encore decks are missing in the simulator. \n They have been swapped with another version of the same card.\n Please verify changes on the right before importing.";
        }

        if (this->match_changed_cards.size() == 50){
            to_display = "WEIRD : All cards found in Encore decks are missing in the simulator. \n They have been swapped with another version of the same card. \n Verify the complete deck before importing.";
        }
        this->ui->importStatusLabel->setText(QString().fromStdString(to_display));
        int card_width = 280;
        int card_height = 400;
        int text_height = 30;
        this->ui->scrollArea->setVisible(true);
        this->ui->cardComparisonWidget->setVisible(true);
        this->ui->startImport->setVisible(true);
        std::map<std::string,std::string>::iterator it;
        for (it = this->match_changed_cards.begin(); it != this->match_changed_cards.end(); it++){
            QWidget* final_widget = new QWidget();
            QHBoxLayout* qblayout = new QHBoxLayout(final_widget);
            qblayout->setContentsMargins(0,0,0,0);
            QWidget* init_card_widget = new QWidget();
            QVBoxLayout* init_card_layout = new QVBoxLayout(init_card_widget);
            init_card_layout->setContentsMargins(0,0,0,0);
            QWidget* final_card_widget = new QWidget();
            QVBoxLayout* final_card_layout = new QVBoxLayout(final_card_widget);
            final_card_layout->setContentsMargins(0,0,0,0);
            std::string init_key = (*it).first;
            std::string final_key = (*it).second;
            QPixmap* initpixmap = this->image_cards.at(init_key);
            QPixmap* finalpixmap = this->image_cards.at(final_key);
            QLabel* textLabel = new QLabel(">");
            textLabel->setFixedWidth(15);
            QFont font = textLabel->font(); // Get current font
            font.setPointSize(16);      // Set font size to 16
            font.setBold(true);         // Make the font bold

            // Apply the font to the label
            textLabel->setFont(font);



            QLabel* init_card_code_label = new QLabel(QString().fromStdString(init_key));
            init_card_code_label->setFixedWidth(card_width);
            init_card_code_label->setFixedHeight(text_height);
            init_card_code_label->setAlignment(Qt::AlignCenter);
            QLabel* final_card_code_label = new QLabel(QString().fromStdString(final_key));
            final_card_code_label->setFixedWidth(card_width);
            final_card_code_label->setFixedHeight(text_height);
            final_card_code_label->setAlignment(Qt::AlignCenter);
            QLabel* init_imgLabel = new QLabel();
            init_imgLabel->setScaledContents( true );
            init_imgLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
            init_imgLabel->setPixmap(*initpixmap);
            init_imgLabel->setFixedSize(card_width, card_height);

            QLabel* final_imgLabel = new QLabel();
            final_imgLabel->setScaledContents( true );
            final_imgLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
            final_imgLabel->setPixmap(*finalpixmap);
            final_imgLabel->setFixedSize(card_width, card_height);

            init_card_layout->addWidget(init_imgLabel);
            init_card_layout->addWidget(init_card_code_label);

            final_card_layout->addWidget(final_imgLabel);
            final_card_layout->addWidget(final_card_code_label);



            init_card_widget->setFixedHeight(card_height+text_height);
            init_card_widget->setFixedWidth(card_width);
            final_card_widget->setFixedHeight(card_height+text_height);
            final_card_widget->setFixedWidth(card_width);

            //qblayout->setAlignment(Qt::AlignCenter);
            qblayout->addWidget(init_card_widget);
            qblayout->addWidget(textLabel);
            qblayout->addWidget(final_card_widget);
            final_widget->setFixedHeight(card_height+text_height+10);
            final_widget->setFixedWidth(this->ui->cardComparisonWidget->width());

            QListWidgetItem *item = new QListWidgetItem();
            this->ui->cardComparisonWidget->addItem(item);
            this->ui->cardComparisonWidget->setItemWidget(item,final_widget);
            item->setSizeHint( final_widget->sizeHint() );
        }
        //this->ui->encoreDecksList->setText(QString().fromStdString(base_deck_to_display));
        //this->ui->encoreDecksList_2->setText(QString().fromStdString(base_deck_to_display_new));
    }
}

void importdeck::AddToSimu(){
    std::vector<std::string> final_list = {};
    std::vector<std::string> decksplit = this->deck_final_card_list;
    if (decksplit.size() > 0){
        for (std::string card : decksplit){
            if (trim(card) != ""){
                final_list.push_back(trim(card));
            }
        }
    }

    if (final_list.size() == 50){
        bool added = DataLoader::GetInstance()->AddDeckToSimulator(this->deck_name,this->deck_date,final_list);
        if (added){
            QMessageBox dlg = QMessageBox(this);
            dlg.setWindowTitle("Import successful");
            dlg.setText("Import successful ! Deck should be available in this tool or in the simulator");
            dlg.exec();
            DataLoader::GetInstance()->getDecks()->insert_or_assign(deck_name, new Deck(deck_name,deck_date));
            DataLoader::GetInstance()->LoadDeckFromList(final_list,deck_name);
        }
        else {
            QMessageBox dlg = QMessageBox(this);
            dlg.setWindowTitle("Import failed");
            dlg.setText("All cards have been found, but import failed. Simulator files are back to previous state. Please contact this tool developer");
            dlg.exec();
            // transfer backuped file to prefs file to reset changes
    }

    }
}


void importdeck::SwitchToMainMenu(){
    MainWindow* mw = getMainWindow();
    mw->SwitchToMainMenu();
}


