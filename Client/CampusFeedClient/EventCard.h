#ifndef EVENTCARD_H
#define EVENTCARD_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMenu>
#include <QAction>

#include "Event.h"
#include "ICard.h"

namespace Ui {
class EventCard;
}

class EventCard : public QWidget, public ICard
{
    Q_OBJECT

public:
    explicit EventCard(QWidget *parent = nullptr);
    ~EventCard();

    void setEvent(const Event &event);

    // ICard
    void refresh() override;
    QString cardType() const override;

signals:
    void editRequested(const Event &event);
    void deleteRequested(int eventId);

private slots:

    // Interaction Buttons
    void onInterestedClicked();
    void onNotInterestedClicked();
    void onGoingClicked();

    void onReactionSaved(QNetworkReply *reply);
    void onCurrentReactionFetched(QNetworkReply *reply);

    // Menu
    void onMenuButtonClicked();
    void onEditClicked();
    void onDeleteClicked();

private:
    void sendInteraction(const QString &type);
    void fetchCurrentInteraction();
    void applyInteractionState(const QString &type);

private:
    Ui::EventCard *ui;

    QNetworkAccessManager *networkManager;

    Event currentEvent;

    int eventId = -1;
};

#endif // EVENTCARD_H