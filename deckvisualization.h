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
    void PickDeck();
    void ClearDeckWidget();
    void UnlockCardPanel();
    void LockCardPanel(Card c);
    void ShowCardToSidePanel(Card c);
    void DisplayPickedDeck();

private:
    Ui::deckvisualization *ui;
    std::string picked_deck;
    bool lock_card_panel;

public slots:
    void SwitchToMainMenu();

};

#endif // DECKVISUALIZATION_H
