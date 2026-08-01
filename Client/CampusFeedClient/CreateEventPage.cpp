#include "CreateEventPage.h"
#include "ui_CreateEventPage.h"

#include "LoginPage.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QDateTime>

CreateEventPage::CreateEventPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreateEventPage)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground, true);

    //----------------------------------
    // Network Manager
    //----------------------------------

    networkManager = new QNetworkAccessManager(this);

    connect(networkManager,
            &QNetworkAccessManager::finished,
            this,
            &CreateEventPage::onReplyFinished);

    //----------------------------------
    // Navigation & Publish Button
    //----------------------------------
    // NOTE: on_backButton_clicked(), on_navFeedButton_clicked(),
    // on_navNotificationsButton_clicked(), on_navProfileButton_clicked()
    // and on_publishButton_clicked() are already auto-connected by Qt's
    // naming convention (ui->setupUi() calls connectSlotsByName(), which
    // wires QPushButton "publishButton"'s clicked() signal straight to
    // on_publishButton_clicked() because the names match). Do NOT add a
    // manual connect() for them here - that was the cause of the
    // "one click creates two events" bug (see Phase 2 fix notes).

    //----------------------------------
    // Default Values
    //----------------------------------

    ui->eventDateEdit->setDate(QDate::currentDate());
    ui->eventTimeEdit->setTime(QTime::currentTime());
}

CreateEventPage::~CreateEventPage()
{
    delete ui;
}

void CreateEventPage::newEvent()
{
    editMode = false;
    editingEventId = -1;

    clearForm();

    ui->publishButton->setEnabled(true);
    ui->publishButton->setText("Publish Event");
}

void CreateEventPage::editEvent(const Event &event)
{
    editMode = true;
    editingEventId = event.id;

    //----------------------------------
    // Fill Form
    //----------------------------------

    ui->titleLineEdit->setText(event.title);

    ui->descriptionTextEdit->setPlainText(
        event.description);

    ui->locationLineEdit->setText(
        event.location);

    ui->registrationLinkLineEdit->setText(
        event.registrationLink);

    //----------------------------------
    // Category
    //----------------------------------

    int index =
        ui->categoryComboBox->findText(
            event.category);

    if(index != -1)
    {
        ui->categoryComboBox->setCurrentIndex(index);
    }

    //----------------------------------
    // Date & Time
    //----------------------------------

    QDateTime dt =
        QDateTime::fromString(
            event.eventDateTime,
            "yyyy-MM-dd HH:mm:ss");

    if(dt.isValid())
    {
        ui->eventDateEdit->setDate(dt.date());
        ui->eventTimeEdit->setTime(dt.time());
    }

    //----------------------------------
    // UI
    //----------------------------------

    ui->publishButton->setEnabled(true);
    ui->publishButton->setText("Update Event");
}

void CreateEventPage::on_backButton_clicked()
{
    emit backRequested();
}

void CreateEventPage::on_navFeedButton_clicked()
{
    emit feedRequested();
}

void CreateEventPage::on_navNotificationsButton_clicked()
{
    emit notificationsRequested();
}

void CreateEventPage::on_navProfileButton_clicked()
{
    emit profileRequested();
}

void CreateEventPage::on_publishButton_clicked()
{
    //----------------------------------
    // Read Form
    //----------------------------------

    QString title =
        ui->titleLineEdit->text().trimmed();

    QString description =
        ui->descriptionTextEdit->toPlainText().trimmed();

    QString category =
        ui->categoryComboBox->currentText();

    QString location =
        ui->locationLineEdit->text().trimmed();

    QString registrationLink =
        ui->registrationLinkLineEdit->text().trimmed();

    //----------------------------------
    // Validation
    //----------------------------------

    if(title.isEmpty() ||
        description.isEmpty() ||
        location.isEmpty())
    {
        QMessageBox::warning(
            this,
            "CampusFeed",
            "Please fill all required fields.");

        return;
    }

    //----------------------------------
    // Date & Time
    //----------------------------------

    QDateTime dateTime(
        ui->eventDateEdit->date(),
        ui->eventTimeEdit->time());

    QString eventDate =
        dateTime.toString("yyyy-MM-dd HH:mm:ss");

    //----------------------------------
    // JSON Body
    //----------------------------------

    QJsonObject json;

    json["title"] = title;
    json["description"] = description;
    json["category"] = category;
    json["location"] = location;
    json["event_date"] = eventDate;
    json["registration_link"] = registrationLink;
    json["image_path"] = "";

    QByteArray body =
        QJsonDocument(json).toJson();

    //----------------------------------
    // Request
    //----------------------------------
    // Disabled immediately so a double-click (or an impatient extra
    // click while the network request is still in flight) can never
    // fire a second POST/PUT - re-enabled in onReplyFinished().

    ui->publishButton->setEnabled(false);

    QNetworkRequest request;

    if(editMode)
    {
        request.setUrl(
            QUrl(BASE_URL +
                 "/events/" +
                 QString::number(editingEventId)));

        request.setHeader(
            QNetworkRequest::ContentTypeHeader,
            "application/json");

        request.setRawHeader(
            "Authorization",
            ("Bearer " + LoginPage::token).toUtf8());

        networkManager->put(request, body);
    }
    else
    {
        request.setUrl(
            QUrl(BASE_URL + "/events"));

        request.setHeader(
            QNetworkRequest::ContentTypeHeader,
            "application/json");

        request.setRawHeader(
            "Authorization",
            ("Bearer " + LoginPage::token).toUtf8());

        networkManager->post(request, body);
    }
}

void CreateEventPage::onReplyFinished(QNetworkReply *reply)
{
    QByteArray response =
        reply->readAll();

    //----------------------------------
    // Error
    //----------------------------------

    if(reply->error() != QNetworkReply::NoError)
    {
        QMessageBox::critical(
            this,
            "CampusFeed",
            response);

        ui->publishButton->setEnabled(true);

        reply->deleteLater();
        return;
    }

    //----------------------------------
    // Success
    //----------------------------------

    bool wasEditing = editMode;

    QMessageBox::information(
        this,
        "CampusFeed",
        wasEditing
            ? "Event updated successfully!"
            : "Event created successfully!");

    clearForm();

    editMode = false;
    editingEventId = -1;

    ui->publishButton->setEnabled(true);
    ui->publishButton->setText("Publish Event");

    if(wasEditing)
        emit eventUpdated();
    else
        emit eventCreated();

    emit backRequested();

    reply->deleteLater();
}

void CreateEventPage::clearForm()
{
    ui->titleLineEdit->clear();

    ui->descriptionTextEdit->clear();

    ui->locationLineEdit->clear();

    ui->registrationLinkLineEdit->clear();

    ui->categoryComboBox->setCurrentIndex(0);

    ui->eventDateEdit->setDate(
        QDate::currentDate());

    ui->eventTimeEdit->setTime(
        QTime::currentTime());
}
