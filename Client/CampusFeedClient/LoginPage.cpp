#include "LoginPage.h"
#include "ui_Loginpage.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QDebug>

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginPage)
{
    ui->setupUi(this);

    networkManager = new QNetworkAccessManager(this);

    connect(networkManager,
            &QNetworkAccessManager::finished,
            this,
            &LoginPage::onReplyFinished);

    connect(ui->loginButton,
            &QPushButton::clicked,
            this,
            &LoginPage::onLoginClicked);
    connect(ui->signupButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                emit signupRequested();
            });

}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::onLoginClicked()
{
    QString email = ui->emailEdit->text();
    QString password = ui->passwordEdit->text();

    if(email.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this,
                             "Error",
                             "Please enter email and password.");
        return;
    }

    QNetworkRequest request(QUrl("http://localhost:18080/login"));

    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json"
        );

    QJsonObject json;

    json["email"] = email;
    json["password"] = password;

    networkManager->post(
        request,
        QJsonDocument(json).toJson()
        );
}

void LoginPage::onReplyFinished(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();

    qDebug() << response;

    if(reply->error() != QNetworkReply::NoError)
    {
        QMessageBox::critical(this,
                              "Login Failed",
                              response);

        reply->deleteLater();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(response);

    if(!doc.isObject())
    {
        QMessageBox::warning(this,
                             "Error",
                             "Invalid JSON received.");

        reply->deleteLater();
        return;
    }

    QJsonObject obj = doc.object();

    QString token = obj["token"].toString();

    QString message = obj["message"].toString();

    QMessageBox::information(this,
                             "Success",
                             message);

    emit loginSuccessful();

    reply->deleteLater();
}
