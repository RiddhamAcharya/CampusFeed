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
    // Navigation Buttons
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
    // Default Date & Time
    //----------------------------------

    ui->eventDateEdit->setDate(QDate::currentDate());

    ui->eventTimeEdit->setTime(QTime::currentTime());
}

CreateEventPage::~CreateEventPage()
{
    delete ui;
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
    // Read UI
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
    // Build DateTime
    //----------------------------------

    QDateTime dateTime(
        ui->eventDateEdit->date(),
        ui->eventTimeEdit->time());

    QString eventDate =
        dateTime.toString("yyyy-MM-dd HH:mm:ss");

    //----------------------------------
    // JSON
    //----------------------------------

    QJsonObject json;

    json["title"] = title;
    json["description"] = description;
    json["category"] = category;
    json["location"] = location;
    json["event_date"] = eventDate;
    json["registration_link"] = registrationLink;

    // Image upload will be implemented later
    json["image_path"] = "";

    //----------------------------------
    // Request
    //----------------------------------

    QNetworkRequest request(
        QUrl(BASE_URL + "/events"));

    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json");

    request.setRawHeader(
        "Authorization",
        ("Bearer " + LoginPage::token).toUtf8());

    //----------------------------------
    // Send
    //----------------------------------

    networkManager->post(
        request,
        QJsonDocument(json).toJson());
}
void CreateEventPage::onReplyFinished(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();

    //----------------------------------
    // Network Error
    //----------------------------------

    if(reply->error() != QNetworkReply::NoError)
    {
        QMessageBox::critical(
            this,
            "Create Event",
            response);

        reply->deleteLater();
        return;
    }

    //----------------------------------
    // Success
    //----------------------------------

    QMessageBox::information(
        this,
        "CampusFeed",
        "Event created successfully!");

    //----------------------------------
    // Clear Form
    //----------------------------------

    ui->titleLineEdit->clear();

    ui->descriptionTextEdit->clear();

    ui->locationLineEdit->clear();

    ui->registrationLinkLineEdit->clear();

    ui->categoryComboBox->setCurrentIndex(0);

    ui->eventDateEdit->setDate(QDate::currentDate());

    ui->eventTimeEdit->setTime(QTime::currentTime());

    //----------------------------------
    // Refresh Dashboard
    //----------------------------------

    emit eventCreated();

    //----------------------------------
    // Back to Feed
    //----------------------------------

    emit backRequested();

    reply->deleteLater();
}