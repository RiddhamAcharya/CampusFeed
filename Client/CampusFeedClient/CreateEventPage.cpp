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

    //----------------------------------
    // Network Manager
    //----------------------------------

    networkManager = new QNetworkAccessManager(this);

    connect(networkManager,
            &QNetworkAccessManager::finished,
            this,
            &CreateEventPage::onReplyFinished);

    //----------------------------------
    // Navigation
    //----------------------------------

    connect(ui->backButton,
            &QPushButton::clicked,
            this,
            &CreateEventPage::on_backButton_clicked);

    connect(ui->navFeedButton,
            &QPushButton::clicked,
            this,
            &CreateEventPage::on_navFeedButton_clicked);

    connect(ui->navNotificationsButton,
            &QPushButton::clicked,
            this,
            &CreateEventPage::on_navNotificationsButton_clicked);

    connect(ui->navProfileButton,
            &QPushButton::clicked,
            this,
            &CreateEventPage::on_navProfileButton_clicked);

    //----------------------------------
    // Publish Button
    //----------------------------------

    connect(ui->publishButton,
            &QPushButton::clicked,
            this,
            &CreateEventPage::on_publishButton_clicked);

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
