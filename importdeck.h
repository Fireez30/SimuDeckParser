#ifndef IMPORTDECK_H
#define IMPORTDECK_H

#include <QWidget>
#include "mainwindow.h"
namespace Ui {
class importdeck;
}

class importdeck : public QWidget
{
    Q_OBJECT

public:
    explicit importdeck(QWidget *parent = nullptr);
    ~importdeck();
    void switchToUi(const QString &uiFilePath);

private:
    Ui::importdeck *ui;
    MainWindow parent;
private slots:
    void ImportLink();
    void SwitchToMainMenu();
};

#endif // IMPORTDECK_H
