#ifndef APISERVICE_H
#define APISERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>

#include "Event.h"

class ApiService : public QObject
{
    Q_OBJECT

public:
    explicit ApiService(QObject *parent = nullptr);

    void fetchEvents();

signals:
    void eventsReceived(QList<Event> events);
    void networkError(QString message);

private:
    QList<Event> parseEvents(const QByteArray &response);

    QNetworkAccessManager *networkManager;

    const QString BASE_URL = "http://127.0.0.1:18080";
};

#endif // APISERVICE_H