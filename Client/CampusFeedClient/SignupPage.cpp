#include "SignupPage.h"
#include "ui_SignupPage.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

SignUpPage::SignUpPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SignUpPage)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground, true);

    selectedRole = "student";

    networkManager = new QNetworkAccessManager(this);

    connect(networkManager,
            &QNetworkAccessManager::finished,
            this,
            &SignUpPage::onReplyFinished);

    // Register Button
    connect(ui->registerButton,
            &QPushButton::clicked,
            this,
            &SignUpPage::onSignupClicked);

    // Login Redirect
    connect(ui->loginRedirectButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                emit loginRequested();
            });

    // Browse Image
    connect(ui->browseButton,
            &QPushButton::clicked,
            this,
            &SignUpPage::onBrowseClicked);

    // Role Buttons
    connect(ui->studentRoleButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                selectedRole = "student";
            });

    connect(ui->organizerRoleButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                selectedRole = "organizer";
            });

    connect(ui->adminRoleButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                selectedRole = "admin";
            });
}

SignUpPage::~SignUpPage()
{
    delete ui;
}

void SignUpPage::onBrowseClicked()
{
    QString file = QFileDialog::getOpenFileName(
        this,
        "Select Profile Image",
        "",
        "Images (*.png *.jpg *.jpeg)");

    if(file.isEmpty())
        return;

    profileImagePath = file;

    ui->profileImageEdit->setText(file);
}

void SignUpPage::onSignupClicked()
{
    if(ui->fullNameEdit->text().isEmpty() ||
        ui->emailEdit->text().isEmpty() ||
        ui->passwordEdit->text().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Missing Information",
            "Please fill all required fields.");

        return;
    }

    QNetworkRequest request(QUrl("http://localhost:18080/signup"));

    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json");

    QJsonObject json;

    json["full_name"] = ui->fullNameEdit->text();
    json["email"] = ui->emailEdit->text();
    json["password"] = ui->passwordEdit->text();
    json["role"] = selectedRole;
    json["institution"] = ui->institutionEdit->text();
    json["profile_image"] = profileImagePath;

    networkManager->post(
        request,
        QJsonDocument(json).toJson());
}

void SignUpPage::onReplyFinished(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();

    qDebug() << response;

    if(reply->error() != QNetworkReply::NoError)
    {
        QMessageBox::critical(
            this,
            "Signup Failed",
            QString(response));

        reply->deleteLater();
        return;
    }

    QMessageBox::information(
        this,
        "Success",
        "Account created successfully!");

    emit signupSuccessful();

    reply->deleteLater();
}