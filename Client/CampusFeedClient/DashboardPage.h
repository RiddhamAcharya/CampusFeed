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
    explicit DashBoardPage(QWidget *parent =nullptr);
    ~DashBoardPage();

private slots:

    void fetchEvents();
    void onEventsReceived();
    void filterEvents();

private:

    void parseEvents(const QByteArray &response);
    void displayEvents(const QList<Event> &events);


    Ui::DashBoardPage *ui;

    QNetworkAccessManager *networkManager;

    QList<Event> events;

    QString currentCategory = "All";
    QString currentSearch = "";

    const QString BASE_URL = "http://127.0.0.1:18080";

signals:
    void feedRequested();
    void notificationsRequested();
    void profileRequested();
};

#endif