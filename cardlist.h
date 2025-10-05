#ifndef CARDLIST_H
#define CARDLIST_H

#include <QWidget>

namespace Ui {
class cardlist;
}

class cardlist : public QWidget
{
    Q_OBJECT

public:
    explicit cardlist(QWidget *parent = nullptr);
    ~cardlist();

private:
    Ui::cardlist *ui;
};

#endif // CARDLIST_H
