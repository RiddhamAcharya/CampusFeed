#include "DashboardPage.h"
#include "ui_DashboardPage.h"
#include "EventCard.h"
#include "LoginPage.h"
#include <QMessageBox>

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

#include "TimeUtils.h"

DashBoardPage::DashBoardPage(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::DashBoardPage)
{
    ui->setupUi(this);

    // A plain QWidget does not paint its own "background-color" stylesheet
    // rule unless this attribute is set - without it, any child frame that
    // doesn't set its own background (pageHeaderFrame, searchFrame,
    // chipsFrame) shows the raw system background through instead of the
    // white background defined in DashboardPage.ui. This is what caused
    // the page to look black right after login.
    setAttribute(Qt::WA_StyledBackground, true);

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

    //-----------------------------
    // Create Event Button
    // ----------------------------

    connect(ui->createEventButton,
            &QPushButton::clicked,
            this,
            [=]()
            {
                emit createEventRequested();
            });

}

DashBoardPage::~DashBoardPage()
{
    delete ui;
}

void DashBoardPage::fetchEvents()
{
    // Re-checked here (rather than only in the constructor) because
    // DashboardPage is created once at app startup, before the user has
    // logged in - LoginPage::role isn't known yet at construction time.
    // fetchEvents() runs every time this page becomes relevant (after
    // login, after navigating back, after create/update), so this stays
    // accurate for whoever is actually logged in.
    ui->createEventButton->setVisible(
        LoginPage::role == "organizer" || LoginPage::role == "admin");

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

    try
    {
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

        event.organizerId =
            obj.contains("organizer_id")
                ? obj["organizer_id"].toInt()
                : -1;

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
        // Created At / Time Ago
        //-----------------------------

        event.createdAt =
            obj.contains("created_at")
                ? obj["created_at"].toString()
                : "";

        event.timeAgo = TimeUtils::relativeTime(event.createdAt);

        events.append(event);
    }

    //-----------------------------
    // Sort Newest First (by when the post was created, like a
    // real social feed - not by the upcoming event_date)
    //-----------------------------

    std::sort(events.begin(), events.end(),
              [](const Event &a, const Event &b)
              {
                  return QDateTime::fromString(
                             a.createdAt,
                             "yyyy-MM-dd HH:mm:ss")
                         >
                         QDateTime::fromString(
                             b.createdAt,
                             "yyyy-MM-dd HH:mm:ss");
              });

    qDebug() << "================================";
    qDebug() << "Events Parsed:" << events.size();

    for (const Event &event : events)
    {
        qDebug() << event.title << event.eventDateTime;
    }

    qDebug() << "================================";
    }
    catch (const std::exception &e)
    {
        QMessageBox::warning(
            this,
            "CampusFeed",
            QString("Something went wrong while loading the feed: %1")
                .arg(e.what()));
        return;
    }

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
        EventCard *card = new EventCard(this);

        card->setEvent(event);

        connect(card,
                &EventCard::deleteRequested,
                this,
                &DashBoardPage::onDeleteEventRequested);

        connect(card,
                &EventCard::editRequested,
                this,
                &DashBoardPage::onEditEventRequested);

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

void DashBoardPage::onDeleteEventRequested(int eventId)
{
    QNetworkRequest request(
        QUrl(BASE_URL + "/events/" + QString::number(eventId)));

    request.setRawHeader(
        "Authorization",
        ("Bearer " + LoginPage::token).toUtf8());

    QNetworkReply *reply =
        networkManager->deleteResource(request);

    connect(reply,
            &QNetworkReply::finished,
            this,
            [this, reply]()
            {
                onDeleteReplyFinished(reply);
            });
}

void DashBoardPage::onDeleteReplyFinished(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();

    if(reply->error() != QNetworkReply::NoError)
    {
        QMessageBox::warning(
            this,
            "Delete Event",
            response);

        reply->deleteLater();
        return;
    }

    QMessageBox::information(
        this,
        "CampusFeed",
        "Event deleted successfully.");

    reply->deleteLater();

    fetchEvents();
}

void DashBoardPage::onEditEventRequested(const Event &event)
{
    emit editEventRequested(event);
}