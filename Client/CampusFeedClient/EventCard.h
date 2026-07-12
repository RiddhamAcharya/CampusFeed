#ifndef EVENTCARD_H
#define EVENTCARD_H

#include <QWidget>
#include "Event.h"

namespace Ui {
class EventCard;
}

class EventCard : public QWidget
{
    Q_OBJECT

public:
    explicit EventCard(QWidget *parent = nullptr);
    ~EventCard();

    void setEvent(const Event &event);

private:
    Ui::EventCard *ui;
};

#endif