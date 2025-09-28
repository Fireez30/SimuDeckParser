#include <QLabel>
#include <QEvent>
#include <QDebug>

class HoverLabel : public QLabel {
    Q_OBJECT
public:
    explicit HoverLabel(QWidget *parent = nullptr) : QLabel(parent) {}


signals:
    void mouseEntered();
    void mouseLeft();

protected:
    bool event(QEvent *event) override {
        switch (event->type()) {
        case QEvent::Enter:
            emit mouseEntered();
            break;
        case QEvent::Leave:
            emit mouseLeft();
            break;
        default:
            break;
        }
        return QLabel::event(event);
    }
};
