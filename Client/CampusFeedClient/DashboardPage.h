#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>

#include "Event.h"

namespace Ui {
class DashBoardPage;
}

class NotificationPage;
class ProfilePage;

class DashBoardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashBoardPage(QWidget *parent = nullptr);
    ~DashBoardPage();

public slots:
    void fetchEvents();

private slots:

    void onEventsReceived();
    void filterEvents();

    // CRUD
    void onDeleteEventRequested(int eventId);
    void onDeleteReplyFinished(QNetworkReply *reply);

    void onEditEventRequested(const Event &event);

private:

    void parseEvents(const QByteArray &response);
    void displayEvents(const QList<Event> &events);

    Ui::DashBoardPage *ui;

    QNetworkAccessManager *networkManager;

    QList<Event> events;

    QString currentCategory = "";
    QString currentSearch = "";


    const QString BASE_URL = "http://127.0.0.1:18080";

signals:

    void feedRequested();
    void notificationsRequested();
    void profileRequested();
    void createEventRequested();

    void editEventRequested(const Event &event);
};

#endif