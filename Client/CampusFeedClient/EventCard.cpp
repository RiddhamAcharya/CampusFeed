#include "EventCard.h"
#include "ui_EventCard.h"
#include "Event.h"
#include "LoginPage.h"

#include <QPixmap>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

EventCard::EventCard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EventCard)
{
    ui->setupUi(this);

    ui->descriptionLabel->setWordWrap(true);
    ui->eventImageLabel->setScaledContents(true);

    networkManager = new QNetworkAccessManager(this);

    connect(ui->interestedButton,
            &QPushButton::clicked,
            this,
            &EventCard::onInterestedClicked);

    connect(ui->notInterestedButton,
            &QPushButton::clicked,
            this,
            &EventCard::onNotInterestedClicked);

    connect(ui->goingButton,
            &QPushButton::clicked,
            this,
            &EventCard::onGoingClicked);

    connect(ui->menuButton,
            &QPushButton::clicked,
            this,
            &EventCard::onMenuButtonClicked);
}

EventCard::~EventCard()
{
    delete ui;
}

void EventCard::setEvent(const Event &event)
{
    currentEvent = event;
    eventId = event.id;

    // Header
    ui->organizerLabel->setText(event.organizer);
    ui->titleLabel->setText(event.title);
    ui->descriptionLabel->setText(event.description);
    ui->timeLabel->setText(
        event.timeAgo.isEmpty() ? "Recently" : event.timeAgo);

    // Details Row
    ui->dateLabel->setText(event.date);
    ui->eventTimeLabel->setText(event.time);
    ui->locationLabel->setText(event.location);
    ui->categoryLabel->setText(event.category);

    // Only the event's creator (or an admin) should see the ⋮ Edit/Delete
    // menu - everyone else just sees the read-only card.
    bool canManage =
        (event.organizerId != -1 &&
         event.organizerId == LoginPage::userId) ||
        LoginPage::role == "admin";

    ui->menuButton->setVisible(canManage);

    // Event Image
    if (!event.imagePath.isEmpty())
    {
        QPixmap pixmap(event.imagePath);

        if (!pixmap.isNull())
        {
            ui->eventImageLabel->setPixmap(
                pixmap.scaled(
                    ui->eventImageLabel->size(),
                    Qt::KeepAspectRatioByExpanding,
                    Qt::SmoothTransformation));
        }
    }

    // Reset button states
    ui->interestedButton->setChecked(false);
    ui->notInterestedButton->setChecked(false);
    ui->goingButton->setChecked(false);

    // Ask the server what this user already picked for this event
    fetchCurrentInteraction();
}

void EventCard::onInterestedClicked()
{
    sendInteraction("interested");
}

void EventCard::onNotInterestedClicked()
{
    sendInteraction("not_interested");
}

void EventCard::onGoingClicked()
{
    sendInteraction("going");
}

void EventCard::sendInteraction(const QString &type)
{
    if (eventId == -1 || LoginPage::userId == -1)
    {
        qDebug() << "Cannot send interaction: missing eventId or userId";
        return;
    }

    // Instant visual feedback, corrected later if the request fails
    applyInteractionState(type);

    QNetworkRequest request(QUrl("http://localhost:18080/interactions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + LoginPage::token).toUtf8());

    QJsonObject json;
    json["user_id"] = LoginPage::userId;
    json["event_id"] = eventId;
    json["interaction_type"] = type;

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onReactionSaved(reply);
    });
}

void EventCard::onReactionSaved(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Failed to save interaction:" << response;
        // Re-sync with the server so the button state reflects reality
        fetchCurrentInteraction();
    }

    reply->deleteLater();
}

void EventCard::fetchCurrentInteraction()
{
    if (eventId == -1 || LoginPage::userId == -1)
        return;

    QUrl url(QString("http://localhost:18080/interactions/%1/%2")
                 .arg(eventId)
                 .arg(LoginPage::userId));

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + LoginPage::token).toUtf8());

    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onCurrentReactionFetched(reply);
    });
}

void EventCard::onCurrentReactionFetched(QNetworkReply *reply)
{

    QByteArray response = reply->readAll();
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Failed to fetch interaction:" << response;
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (!doc.isObject())
        return;

    QString type = doc.object()["interaction_type"].toString();
    applyInteractionState(type);
}

void EventCard::applyInteractionState(const QString &type)
{
    const QString selectedStyle =
        "QPushButton {"
        "    background-color: #4C6FE0;"
        "    color: #FFFFFF;"
        "    border: 1px solid #4C6FE0;"
        "    border-radius: 12px;"
        "    font-size: 11pt;"
        "    font-weight: bold;"
        "}";

    const QString unselectedStyle =
        "QPushButton {"
        "    background-color: #FFFFFF;"
        "    color: #4C6FE0;"
        "    border: 1px solid #4C6FE0;"
        "    border-radius: 12px;"
        "    font-size: 11pt;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #F2F5FF;"
        "}";

    ui->interestedButton->setChecked(type == "interested");
    ui->notInterestedButton->setChecked(type == "not_interested");
    ui->goingButton->setChecked(type == "going");

    ui->interestedButton->setStyleSheet(
        type == "interested" ? selectedStyle : unselectedStyle);
    ui->notInterestedButton->setStyleSheet(
        type == "not_interested" ? selectedStyle : unselectedStyle);
    ui->goingButton->setStyleSheet(
        type == "going" ? selectedStyle : unselectedStyle);
}

void EventCard::onMenuButtonClicked()
{
    QMenu menu(this);

    menu.setStyleSheet(
        "QMenu {"
        "    background-color: white;"
        "    border: 1px solid #D9D9D9;"
        "    border-radius: 8px;"
        "    padding: 6px;"
        "}"
        "QMenu::item {"
        "    color: black;"
        "    padding: 8px 24px;"
        "    border-radius: 6px;"
        "}"
        "QMenu::item:selected {"
        "    background-color: #EEF4FF;"
        "    color: #2563EB;"
        "}"
        );

    QAction *editAction =
        menu.addAction("✏ Edit Event");

    QAction *deleteAction =
        menu.addAction("🗑 Delete Event");

    QAction *selected =
        menu.exec(
            ui->menuButton->mapToGlobal(
                QPoint(0, ui->menuButton->height())));

    if(selected == editAction)
    {
        onEditClicked();
    }
    else if(selected == deleteAction)
    {
        onDeleteClicked();
    }
}

void EventCard::onEditClicked()
{
    emit editRequested(currentEvent);
}

void EventCard::onDeleteClicked()
{
    QMessageBox::StandardButton reply =
        QMessageBox::question(
            this,
            "Delete Event",
            "Are you sure you want to delete this event?",
            QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        emit deleteRequested(eventId);
    }
}

void EventCard::refresh()
{
    fetchCurrentInteraction();
}

QString EventCard::cardType() const
{
    return "Event";
}



