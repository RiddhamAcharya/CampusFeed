#include "DashboardPage.h"
#include "ui_DashboardPage.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

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
            &DashBoardPage::filterEvents);

    // -----------------------
    // Category Chips
    // -----------------------

    connect(ui->chipAll,
            &QPushButton::clicked,
            this,
            [=]()
            {
                currentCategory = "";
                fetchEvents();
            });

    connect(ui->chipTechnology,
            &QPushButton::clicked,
            this,
            [=]()
            {
                currentCategory = "Technology";
                fetchEvents();
            });

    connect(ui->chipWorkshop,
            &QPushButton::clicked,
            this,
            [=]()
            {
                currentCategory = "Workshop";
                fetchEvents();
            });

    connect(ui->chipSeminar,
            &QPushButton::clicked,
            this,
            [=]()
            {
                currentCategory = "Seminar";
                fetchEvents();
            });

    connect(ui->chipSports,
            &QPushButton::clicked,
            this,
            [=]()
            {
                currentCategory = "Sports";
                fetchEvents();
            });

    connect(ui->chipCultural,
            &QPushButton::clicked,
            this,
            [=]()
            {
                currentCategory = "Cultural";
                fetchEvents();
            });

    connect(ui->chipOther,
            &QPushButton::clicked,
            this,
            [=]()
            {
                currentCategory = "Other";
                fetchEvents();
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
            [=]()
            {
                emit notificationsRequested();
            });

    connect(ui->navProfileButton,
            &QPushButton::clicked,
            this,
            [=]()
            {
                emit profileRequested();
            });
}

DashBoardPage::~DashBoardPage()
{
    delete ui;
}

void DashBoardPage::fetchEvents()
{
    QString url = BASE_URL + "/events";

    if (!currentCategory.isEmpty())
    {
        url += "?category=" + currentCategory;
    }

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

    if(parseError.error != QJsonParseError::NoError)
    {
        QMessageBox::warning(this,
                             "JSON Error",
                             parseError.errorString());
        return;
    }

    if(!document.isArray())
    {
        QMessageBox::warning(this,
                             "Backend Error",
                             "Backend did not return a JSON array.");
        return;
    }

    QJsonArray array = document.array();

    for(const QJsonValue &value : array)
    {
        if(!value.isObject())
            continue;

        QJsonObject obj = value.toObject();

        Event event;

        //----------------------------------
        // Basic Information
        //----------------------------------

        event.id = obj["id"].toInt();

        event.title =
            obj["title"].toString();

        event.description =
            obj["description"].toString();

        event.category =
            obj["category"].toString();

        event.location =
            obj["location"].toString();

        event.registrationLink =
            obj["registration_link"].toString();

        //----------------------------------
        // Organizer
        //----------------------------------

        if(obj.contains("organizer"))
            event.organizer =
                obj["organizer"].toString();
        else
            event.organizer =
                "Campus Organizer";

        //----------------------------------
        // Image
        //----------------------------------

        if(obj.contains("image"))
        {
            event.imagePath =
                obj["image"].toString();
        }
        else
        {
            event.imagePath = "";
        }

        //----------------------------------
        // Date & Time
        //----------------------------------

        QString backendDate =
            obj["event_date"].toString();

        QDateTime dateTime =
            QDateTime::fromString(
                backendDate,
                "yyyy-MM-dd HH:mm:ss");

        if(dateTime.isValid())
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

        //----------------------------------
        // Time Ago
        //----------------------------------

        event.timeAgo = "Recently";

        //----------------------------------
        // Save
        //----------------------------------

        events.append(event);
    }

    qDebug() << "================================";
    qDebug() << "Events Parsed :" << events.size();

    for(const Event &event : events)
    {
        qDebug() << event.title;
    }

    qDebug() << "================================";

    displayEvents(events);
}

void DashBoardPage::displayEvents(const QList<Event> &events)
{
    // Hide both cards initially
    ui->cardTechXHackathon->hide();
    ui->cardModernCpp->hide();

    //--------------------------------------
    // Event 1
    //--------------------------------------

    if(events.size() >= 1)
    {
        const Event &event = events.at(0);

        ui->cardTechXHackathon->show();

        ui->organizerNameTechX->setText(event.organizer);
        ui->organizerTimeTechX->setText(event.timeAgo);

        ui->titleTechX->setText(event.title);
        ui->descTechX->setText(event.description);

        ui->dateTechX->setText("📅 " + event.date);
        ui->locationTechX->setText("📍 " + event.location);

        ui->badgeTechX->setText(event.category);

        // Temporary banner
        if(event.imagePath.isEmpty())
        {
            ui->bannerTechX->setText(event.title);
        }
        else
        {
            // Image loading comes later
            ui->bannerTechX->setText(event.title);
        }

        // Avatar initials
        QString initials = "CF";

        QStringList words =
            event.organizer.split(" ",
                                  Qt::SkipEmptyParts);

        if(words.size() >= 2)
        {
            initials =
                words[0].left(1).toUpper() +
                words[1].left(1).toUpper();
        }
        else if(words.size() == 1)
        {
            initials =
                words[0].left(2).toUpper();
        }

        ui->avatarTechX->setText(initials);
    }

    //--------------------------------------
    // Event 2
    //--------------------------------------

    if(events.size() >= 2)
    {
        const Event &event = events.at(1);

        ui->cardModernCpp->show();

        ui->organizerNameCpp->setText(event.organizer);
        ui->organizerTimeCpp->setText(event.timeAgo);

        ui->titleCpp->setText(event.title);
        ui->descCpp->setText(event.description);

        ui->dateCpp->setText("📅 " + event.date);
        ui->timeCpp->setText("🕒 " + event.time);
        ui->locationCpp->setText("📍 " + event.location);

        ui->badgeCpp->setText(event.category);

        if(event.imagePath.isEmpty())
        {
            ui->bannerCpp->setText(event.title);
        }
        else
        {
            ui->bannerCpp->setText(event.title);
        }

        QString initials = "CF";

        QStringList words =
            event.organizer.split(" ",
                                  Qt::SkipEmptyParts);

        if(words.size() >= 2)
        {
            initials =
                words[0].left(1).toUpper() +
                words[1].left(1).toUpper();
        }
        else if(words.size() == 1)
        {
            initials =
                words[0].left(2).toUpper();
        }

        ui->avatarCpp->setText(initials);
    }

    //--------------------------------------
    // No Events
    //--------------------------------------

    if(events.isEmpty())
    {
        QMessageBox::information(
            this,
            "CampusFeed",
            "No events found.");
    }
}

void DashBoardPage::filterEvents()
{
    QString search =
        ui->searchLineEdit->text().trimmed().toLower();

    if(search.isEmpty())
    {
        displayEvents(events);
        return;
    }

    QList<Event> filteredEvents;

    for(const Event &event : events)
    {
        if(event.title.toLower().contains(search) ||
            event.description.toLower().contains(search) ||
            event.organizer.toLower().contains(search) ||
            event.location.toLower().contains(search))
        {
            filteredEvents.append(event);
        }
    }

    displayEvents(filteredEvents);
}