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
private slots:
    void ImportLink();
    void SwitchToMainMenu();
    void AddToSimu();
};

#endif // IMPORTDECK_H
