#ifndef DECKVISUALIZATION_H
#define DECKVISUALIZATION_H

#include <QWidget>
#include "algorithms.h"
namespace Ui {
class deckvisualization;
}

class deckvisualization : public QWidget
{
    Q_OBJECT

public:
    explicit deckvisualization(QWidget *parent = nullptr);
    ~deckvisualization();
    void ClearDeckWidget();
    void DisplayPickedDeck();

private:
    Ui::deckvisualization *ui;
    std::string picked_deck;
    bool lock_card_panel;

public slots:
    void SwitchToMainMenu();
    void PickDeck();
    void ShowCardToSidePanel(Card* c);
    void LockCardPanel(Card* c);
    void UnlockCardPanel();

};

#endif // DECKVISUALIZATION_H
