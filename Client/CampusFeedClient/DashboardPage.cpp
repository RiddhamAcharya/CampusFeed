#include "DashboardPage.h"
#include "ui_DashboardPage.h"
#include "EventCard.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

#include <algorithm>
#include <QDateTime>

DashBoardPage::DashBoardPage(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::DashBoardPage)
{
    ui->setupUi(this);

    networkManager = new QNetworkAccessManager(this);

    // Fetch all events when page opens
    fetchEvents();

    // -----------------------
    // Search
    // -----------------------

    connect(ui->searchLineEdit,
            &QLineEdit::textChanged,
            this,
            [this](const QString &text)
            {
                currentSearch = text;
                filterEvents();
            });
    // Category Chips
    // -----------------------

    connect(ui->chipAll,
            &QPushButton::clicked,
            this,
            [=]()
            {
                currentCategory = "";
                filterEvents();
            });

    connect(ui->chipTechnology,
            &QPushButton::clicked,
            this,
            [=]()
            {
                currentCategory = "Technology";
                filterEvents();
            });

    connect(ui->chipWorkshop,
            &QPushButton::clicked,
            this,
            [=]()
            {
                currentCategory = "Workshop";
                filterEvents();
            });

    connect(ui->chipSeminar,
            &QPushButton::clicked,
            this,
            [=]()
            {
                currentCategory = "Seminar";
                filterEvents();
            });

    connect(ui->chipSports,
            &QPushButton::clicked,
            this,
            [=]()
            {
                currentCategory = "Sports";
                filterEvents();
            });

    connect(ui->chipCultural,
            &QPushButton::clicked,
            this,
            [=]()
            {
                currentCategory = "Cultural";
                filterEvents();
            });

    connect(ui->chipOther,
            &QPushButton::clicked,
            this,
            [=]()
            {
                currentCategory = "Other";
                filterEvents();
            });

    // -----------------------
    // Bottom Navigation
    // -----------------------

    connect(ui->navFeedButton,
            &QPushButton::clicked,
            this,
            [=]()
            {
                emit feedRequested();
            });

    connect(ui->navNotificationsButton,
            &QPushButton::clicked,
            this,
            &DashBoardPage::notificationsRequested);

    connect(ui->navProfileButton,
            &QPushButton::clicked,
            this,
            &DashBoardPage::profileRequested);

}

DashBoardPage::~DashBoardPage()
{
    delete ui;
}

void DashBoardPage::fetchEvents()
{
    QString url = BASE_URL + "/events";

    QNetworkRequest request{QUrl(url)};

    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/json");

    QNetworkReply *reply = networkManager->get(request);

    connect(reply,
            &QNetworkReply::finished,
            this,
            &DashBoardPage::onEventsReceived);
}

void DashBoardPage::onEventsReceived()
{
    QNetworkReply *reply =
        qobject_cast<QNetworkReply *>(sender());

    if(reply == nullptr)
        return;

    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "==============================";
        qDebug() << "Network Error";
        qDebug() << reply->errorString();
        qDebug() << "HTTP Status:"
                 << reply->attribute(
                             QNetworkRequest::HttpStatusCodeAttribute)
                        .toInt();
        qDebug() << "==============================";

        QMessageBox::warning(
            this,
            "Network Error",
            reply->errorString());

        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();

    qDebug() << "Response received:";
    qDebug() << response;

    parseEvents(response);

    reply->deleteLater();
}

void DashBoardPage::parseEvents(const QByteArray &response)
{
    events.clear();

    QJsonParseError parseError;

    QJsonDocument document =
        QJsonDocument::fromJson(response, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        QMessageBox::warning(this,
                             "JSON Error",
                             parseError.errorString());
        return;
    }

    if (!document.isArray())
    {
        QMessageBox::warning(this,
                             "Backend Error",
                             "Backend did not return a JSON array.");
        return;
    }

    QJsonArray array = document.array();

    for (const QJsonValue &value : array)
    {
        if (!value.isObject())
            continue;

        QJsonObject obj = value.toObject();

        Event event;

        //-----------------------------
        // Basic Info
        //-----------------------------

        event.id = obj["id"].toInt();
        event.title = obj["title"].toString();
        event.description = obj["description"].toString();
        event.category = obj["category"].toString();
        event.location = obj["location"].toString();
        event.registrationLink = obj["registration_link"].toString();

        //-----------------------------
        // Organizer
        //-----------------------------

        event.organizer =
            obj.contains("organizer")
                ? obj["organizer"].toString()
                : "Campus Organizer";

        //-----------------------------
        // Image
        //-----------------------------

        event.imagePath =
            obj.contains("image")
                ? obj["image"].toString()
                : "";

        //-----------------------------
        // Date & Time
        //-----------------------------

        QString backendDate =
            obj["event_date"].toString();

        event.eventDateTime = backendDate;

        QDateTime dateTime =
            QDateTime::fromString(
                backendDate,
                "yyyy-MM-dd HH:mm:ss");

        if (dateTime.isValid())
        {
            event.date =
                dateTime.date().toString("dd MMM yyyy");

            event.time =
                dateTime.time().toString("hh:mm AP");
        }
        else
        {
            event.date = backendDate;
            event.time = "";
        }

        //-----------------------------
        // Time Ago
        //-----------------------------

        event.timeAgo = "Recently";

        events.append(event);
    }

    //-----------------------------
    // Sort Newest First
    //-----------------------------

    std::sort(events.begin(), events.end(),
              [](const Event &a, const Event &b)
              {
                  return QDateTime::fromString(
                             a.eventDateTime,
                             "yyyy-MM-dd HH:mm:ss")
                         >
                         QDateTime::fromString(
                             b.eventDateTime,
                             "yyyy-MM-dd HH:mm:ss");
              });

    qDebug() << "================================";
    qDebug() << "Events Parsed:" << events.size();

    for (const Event &event : events)
    {
        qDebug() << event.title << event.eventDateTime;
    }

    qDebug() << "================================";

    filterEvents();
}

void DashBoardPage::displayEvents(const QList<Event> &events)
{
    // Remove previous cards
    QLayoutItem *child;

    while ((child = ui->feedLayout->takeAt(0)) != nullptr)
    {
        if (child->widget())
        {
            child->widget()->deleteLater();
        }

        delete child;
    }

    // No events
    if (events.isEmpty())
    {
        QLabel *label = new QLabel("No events available.");

        label->setAlignment(Qt::AlignCenter);

        label->setStyleSheet(
            "font-size:18px;"
            "color:gray;"
            "padding:30px;"
            );

        ui->feedLayout->addWidget(label);

        return;
    }

    // Create one EventCard per event
    for (const Event &event : events)
    {
        EventCard *card = new EventCard();

        card->setEvent(event);

        ui->feedLayout->addWidget(card);
    }

    // Push cards to top
    ui->feedLayout->addStretch();
}

void DashBoardPage::filterEvents()
{
    QList<Event> filteredEvents;

    QString searchText =
        ui->searchLineEdit->text().trimmed();

    for (const Event &event : events)
    {
        // ---------- Category ----------
        bool categoryMatch =
            currentCategory.isEmpty() ||
            currentCategory == "All" ||
            event.category.compare(currentCategory,
                                   Qt::CaseInsensitive) == 0;

        // ---------- Search ----------
        bool searchMatch =
            searchText.isEmpty() ||
            event.title.contains(searchText, Qt::CaseInsensitive) ||
            event.description.contains(searchText, Qt::CaseInsensitive) ||
            event.location.contains(searchText, Qt::CaseInsensitive);

        if(categoryMatch && searchMatch)
            filteredEvents.append(event);
    }

    displayEvents(filteredEvents);
}