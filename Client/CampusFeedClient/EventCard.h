#ifndef EVENTCARD_H
#define EVENTCARD_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
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

private slots:
    void onInterestedClicked();
    void onNotInterestedClicked();
    void onGoingClicked();
    void onReactionSaved(QNetworkReply *reply);
    void onCurrentReactionFetched(QNetworkReply *reply);

private:
    void sendInteraction(const QString &type);
    void fetchCurrentInteraction();
    void applyInteractionState(const QString &type);

    Ui::EventCard *ui;
    QNetworkAccessManager *networkManager;
    int eventId = -1;
};

#endif // EVENTCARD_H
