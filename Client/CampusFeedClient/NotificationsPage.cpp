#include "NotificationsPage.h"
#include "ui_NotificationsPage.h"
#include "NotificationCard.h"
#include "LoginPage.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMessageBox>
#include <QLabel>
#include <QLayoutItem>
#include <QDebug>

NotificationsPage::NotificationsPage(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::NotificationsPage)
{
    ui->setupUi(this);

    // Same background-painting fix as DashboardPage - see notes there.
    setAttribute(Qt::WA_StyledBackground, true);

    networkManager = new QNetworkAccessManager(this);

    connect(ui->navFeedButton,
            &QPushButton::clicked,
            this,
            &NotificationsPage::on_navFeedButton_clicked);

    connect(ui->navProfileButton,
            &QPushButton::clicked,
            this,
            &NotificationsPage::on_navProfileButton_clicked);

    fetchNotifications();
}

NotificationsPage::~NotificationsPage()
{
    delete ui;
}

void NotificationsPage::on_navFeedButton_clicked()
{
    emit feedRequested();
}

void NotificationsPage::on_navProfileButton_clicked()
{
    emit profileRequested();
}

void NotificationsPage::fetchNotifications()
{
    if (LoginPage::userId == -1)
    {
        // Not logged in - nothing to fetch yet.
        return;
    }

    QUrl url(BASE_URL + "/notifications/" + QString::number(LoginPage::userId));

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + LoginPage::token).toUtf8());

    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, &NotificationsPage::onNotificationsReceived);
}

void NotificationsPage::onNotificationsReceived()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply == nullptr)
        return;

    QByteArray response = reply->readAll();
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Failed to fetch notifications:" << response;
        QMessageBox::warning(
            this,
            "Notifications",
            "Could not load notifications. Please check your connection.");
        return;
    }

    try
    {
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);

        if (parseError.error != QJsonParseError::NoError || !doc.isArray())
        {
            QMessageBox::warning(this, "Notifications", "Unexpected response from server.");
            return;
        }

        displayNotifications(doc.array());
    }
    catch (const std::exception &e)
    {
        QMessageBox::warning(
            this,
            "Notifications",
            QString("Something went wrong: %1").arg(e.what()));
    }
}

void NotificationsPage::displayNotifications(const QJsonArray &array)
{
    // Remove everything currently in the list (old cards + old spacer).
    QLayoutItem *child;

    while ((child = ui->cardsLayout->takeAt(0)) != nullptr)
    {
        if (child->widget())
        {
            child->widget()->deleteLater();
        }

        delete child;
    }

    if (array.isEmpty())
    {
        QLabel *emptyLabel = new QLabel("You're all caught up - no notifications yet.");

        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("font-size:15px; color:#7A7F9A; padding:40px;");

        ui->cardsLayout->addWidget(emptyLabel);
        ui->cardsLayout->addStretch();
        return;
    }

    for (const QJsonValue &value : array)
    {
        if (!value.isObject())
            continue;

        QJsonObject obj = value.toObject();

        int id = obj["id"].toInt();
        QString title = obj["title"].toString();
        QString message = obj["message"].toString();
        bool isRead = obj["is_read"].toInt() != 0;

        QString createdAt =
            obj.contains("created_at")
                ? obj["created_at"].toString()
                : "";

        NotificationCard *card = new NotificationCard(this);

        card->setNotification(id, title, message, createdAt, isRead);

        connect(card, &NotificationCard::clicked, this, &NotificationsPage::onCardClicked);

        ui->cardsLayout->addWidget(card);
    }

    ui->cardsLayout->addStretch();
}

void NotificationsPage::onCardClicked(int notificationId)
{
    markAsRead(notificationId);
}

void NotificationsPage::markAsRead(int notificationId)
{
    QUrl url(BASE_URL + "/notifications/" + QString::number(notificationId) + "/read");

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + LoginPage::token).toUtf8());

    QNetworkReply *reply = networkManager->sendCustomRequest(request, "PATCH");

    connect(reply, &QNetworkReply::finished, this, &NotificationsPage::onMarkReadFinished);
}

void NotificationsPage::onMarkReadFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply == nullptr)
        return;

    bool ok = (reply->error() == QNetworkReply::NoError);
    reply->deleteLater();

    if (!ok)
    {
        qDebug() << "Failed to mark notification as read";
        return;
    }

    // Refresh so the unread dot / styling updates to reflect the read state.
    fetchNotifications();
}
