#ifndef IMPORTDECK_H
#define IMPORTDECK_H

#include <QWidget>
namespace Ui {
class importdeck;
}

class importdeck : public QWidget
{
    Q_OBJECT

public:
    explicit importdeck(QWidget *parent = nullptr);
    ~importdeck();
private:
    Ui::importdeck *ui;
    std::string deck_name;
    std::string deck_date;
    std::vector<std::string> deck_init_card_list;
    std::vector<std::string> deck_final_card_list;
    std::map<std::string,QPixmap*> image_cards;
    std::map<std::string,std::string> match_changed_cards;
    int number_of_images_started;
    int number_of_images_downloaded;
private slots:
    void ImportLink();
    void OnImageDownloaded(std::string init_card_code,QPixmap* buttonImage);
    void SwitchToMainMenu();
    void AddToSimu();
};

#endif // IMPORTDECK_H
