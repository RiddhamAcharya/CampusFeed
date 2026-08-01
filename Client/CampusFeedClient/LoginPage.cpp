#include "LoginPage.h"
#include "ui_Loginpage.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QDebug>

QString LoginPage::token = "";
QString LoginPage::role = "";
int LoginPage::userId = -1;
QString LoginPage::fullName = "";

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginPage)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground, true);

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

void LoginPage::resetForm()
{
    ui->emailEdit->clear();
    ui->passwordEdit->clear();
}

void LoginPage::logout()
{
    LoginPage::token = "";
    LoginPage::role = "";
    LoginPage::userId = -1;
    LoginPage::fullName = "";
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

    ui->loginButton->setEnabled(false);

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

        ui->loginButton->setEnabled(true);

        reply->deleteLater();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(response);

    if(!doc.isObject())
    {
        QMessageBox::warning(this,
                             "Error",
                             "Invalid JSON received.");

        ui->loginButton->setEnabled(true);

        reply->deleteLater();
        return;
    }

    QJsonObject obj = doc.object();

    if (!obj.contains("token") || !obj.contains("user"))
    {
        QMessageBox::warning(this,
                             "Error",
                             "Login response was missing expected data.");

        ui->loginButton->setEnabled(true);

        reply->deleteLater();
        return;
    }

    LoginPage::token = obj["token"].toString();

    QJsonObject user = obj["user"].toObject();

    LoginPage::userId = user["id"].toInt();
    LoginPage::fullName = user["full_name"].toString();
    LoginPage::role = user["role"].toString();

    ui->loginButton->setEnabled(true);

    QString message = obj["message"].toString();

    QMessageBox::information(this,
                             "Success",
                             message);

    emit loginSuccessful();

    reply->deleteLater();
}
