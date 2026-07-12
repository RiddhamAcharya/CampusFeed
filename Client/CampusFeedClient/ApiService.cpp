#include "ApiService.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDateTime>
#include <QDebug>

ApiService::ApiService(QObject *parent)
    : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
}

void ApiService::fetchEvents()
{
    QNetworkRequest request(QUrl(BASE_URL + "/events"));

    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/json");

    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished,
            this,
            [this, reply]()
            {
        if(reply->error() != QNetworkReply::NoError)
        {
            qDebug() << "====================================";
            qDebug() << "Network Error!";
            qDebug() << "Error Code:" << reply->error();
            qDebug() << "Error String:" << reply->errorString();
            qDebug() << "HTTP Status:"
                     << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "Response:";
            qDebug() << reply->readAll();
            qDebug() << "====================================";

            emit networkError(reply->errorString());

            reply->deleteLater();
            return;
        }

                QByteArray response = reply->readAll();

                QList<Event> events = parseEvents(response);

                qDebug() << "Fetched" << events.size() << "events from backend.";

                emit eventsReceived(events);

                reply->deleteLater();
            });
}

QList<Event> ApiService::parseEvents(const QByteArray &response)
{
    QList<Event> events;

    QJsonParseError parseError;

    QJsonDocument doc =
        QJsonDocument::fromJson(response, &parseError);

    if(parseError.error != QJsonParseError::NoError)
    {
        emit networkError(parseError.errorString());
        return events;
    }

    if(!doc.isArray())
    {
        emit networkError("Backend did not return a JSON array.");
        return events;
    }

    QJsonArray array = doc.array();

    for(const QJsonValue &value : array)
    {
        QJsonObject obj = value.toObject();

        Event event;

        event.id = obj["id"].toInt();

        // Temporary until backend returns organizer name
        event.organizer = "Campus Organizer";

        event.title = obj["title"].toString();
        event.description = obj["description"].toString();

        event.category = obj["category"].toString();
        event.location = obj["location"].toString();

        // Placeholder for now.
        // Later we'll download the image from:
        // http://127.0.0.1:18080/uploads/...
        event.imagePath = "";

        event.registrationLink =
            obj["registration_link"].toString();

        QString dateTime =
            obj["event_date"].toString();

        QDateTime dt =
            QDateTime::fromString(dateTime,
                                  "yyyy-MM-dd HH:mm:ss");

        if(dt.isValid())
        {
            event.date =
                dt.date().toString("dd MMM yyyy");

            event.time =
                dt.time().toString("hh:mm AP");
        }
        else
        {
            event.date = dateTime;
            event.time = "";
        }

        // Temporary until backend calculates it
        event.timeAgo = "Recently";

        events.append(event);
    }

    return events;
}