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
private slots:
    void ImportLink();
    void SwitchToMainMenu();
};

#endif // IMPORTDECK_H
