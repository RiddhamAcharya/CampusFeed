#include "ProfilePage.h"
#include "ui_ProfilePage.h"
#include "LoginPage.h"
#include "TimeUtils.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>

ProfilePage::ProfilePage(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::ProfilePage)
{
    ui->setupUi(this);

    // Same background-painting fix as DashboardPage - see notes there.
    setAttribute(Qt::WA_StyledBackground, true);

    networkManager = new QNetworkAccessManager(this);

    connect(ui->navFeedButton,
            &QPushButton::clicked,
            this,
            &ProfilePage::on_navFeedButton_clicked);

    connect(ui->navNotificationsButton,
            &QPushButton::clicked,
            this,
            &ProfilePage::on_navNotificationsButton_clicked);

    connect(ui->logoutButton,
            &QPushButton::clicked,
            this,
            &ProfilePage::on_logoutButton_clicked);

    // The "Edit Profile"/"Institution"/"Change Password"/etc. list rows
    // and the standalone "Edit Profile" button are presentational only
    // for now - no backend endpoint beyond GET/PUT /profile exists for
    // those flows, so they're left as visual affordances rather than
    // wired to fake behaviour.

    refreshProfile();
}

ProfilePage::~ProfilePage()
{
    delete ui;
}

void ProfilePage::on_navFeedButton_clicked()
{
    emit feedRequested();
}

void ProfilePage::on_navNotificationsButton_clicked()
{
    emit notificationsRequested();
}

void ProfilePage::on_logoutButton_clicked()
{
    QMessageBox::StandardButton reply =
        QMessageBox::question(
            this,
            "Log Out",
            "Are you sure you want to log out?",
            QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        emit logoutRequested();
    }
}

void ProfilePage::refreshProfile()
{
    if (LoginPage::userId == -1)
    {
        // Not logged in - nothing to show yet.
        return;
    }

    // Show what we already know immediately (no network round-trip
    // needed for this), then refine with the backend response below.
    applyLocalSessionInfo();

    QUrl url(BASE_URL + "/profile");

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + LoginPage::token).toUtf8());

    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, &ProfilePage::onProfileReplyFinished);

    fetchMyEventsCount();
    fetchInteractionStats();
}

void ProfilePage::applyLocalSessionInfo()
{
    QString name = LoginPage::fullName.isEmpty() ? "Guest" : LoginPage::fullName;

    ui->nameLabel->setText(name);

    QString role = LoginPage::role;
    role = role.isEmpty() ? "Student" : (role.left(1).toUpper() + role.mid(1));
    ui->roleLabel->setText("👤  " + role);

    // Initials for the avatar, e.g. "Riddham Acharya" -> "RA"
    QStringList parts = name.split(' ', Qt::SkipEmptyParts);
    QString initials;

    if (!parts.isEmpty())
        initials += parts.first().at(0).toUpper();

    if (parts.size() > 1)
        initials += parts.last().at(0).toUpper();

    ui->avatarLabel->setText(initials.isEmpty() ? "?" : initials);
}

void ProfilePage::onProfileReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply == nullptr)
        return;

    QByteArray response = reply->readAll();
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Failed to load profile:" << response;
        // Not fatal - the fields already show LoginPage's cached info.
        return;
    }

    try
    {
        QJsonDocument doc = QJsonDocument::fromJson(response);

        if (!doc.isObject())
            return;

        QJsonObject obj = doc.object();

        QString institution = obj["institution"].toString();

        ui->universityLabel->setText(
            institution.isEmpty()
                ? "🏫  Institution not set"
                : "🏫  " + institution);

        QString email = obj["email"].toString();

        // "departmentLabel" doubles as the email row - there's no
        // department field in the schema, and this avoids adding a new
        // widget just to show it.
        ui->departmentLabel->setText("✉  " + email);

        QString createdAt = obj["created_at"].toString();

        ui->verifiedBadge->setText("📅  " + TimeUtils::joinedLabel(createdAt));
    }
    catch (const std::exception &e)
    {
        qDebug() << "Error parsing profile response:" << e.what();
    }
}

void ProfilePage::fetchMyEventsCount()
{
    QUrl url(BASE_URL + "/events/my-events");

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + LoginPage::token).toUtf8());

    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, &ProfilePage::onMyEventsReplyFinished);
}

void ProfilePage::onMyEventsReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply == nullptr)
        return;

    QByteArray response = reply->readAll();
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
        return;

    QJsonDocument doc = QJsonDocument::fromJson(response);

    if (doc.isArray())
    {
        ui->valueCreated->setText(QString::number(doc.array().size()));
    }
}

void ProfilePage::fetchInteractionStats()
{
    pendingInteractionChecks = 0;
    joinedCount = 0;
    interestedCount = 0;

    QUrl url(BASE_URL + "/events");

    QNetworkRequest request(url);

    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, &ProfilePage::onAllEventsForStatsReceived);
}

void ProfilePage::onAllEventsForStatsReceived()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply == nullptr)
        return;

    QByteArray response = reply->readAll();
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
        return;

    QJsonDocument doc = QJsonDocument::fromJson(response);

    if (!doc.isArray())
        return;

    QJsonArray events = doc.array();

    if (events.isEmpty())
    {
        ui->valueEventsJoined->setText("0");
        ui->valueInterested->setText("0");
        return;
    }

    pendingInteractionChecks = events.size();

    for (const QJsonValue &value : events)
    {
        int eventId = value.toObject()["id"].toInt();

        QUrl url(BASE_URL +
                  QString("/interactions/%1/%2")
                      .arg(eventId)
                      .arg(LoginPage::userId));

        QNetworkRequest request(url);
        request.setRawHeader("Authorization", ("Bearer " + LoginPage::token).toUtf8());

        QNetworkReply *checkReply = networkManager->get(request);

        connect(checkReply, &QNetworkReply::finished, this, &ProfilePage::onInteractionCheckReceived);
    }
}

void ProfilePage::onInteractionCheckReceived()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply == nullptr)
        return;

    QByteArray response = reply->readAll();
    reply->deleteLater();

    if (reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument doc = QJsonDocument::fromJson(response);

        if (doc.isObject())
        {
            QString type = doc.object()["interaction_type"].toString();

            if (type == "going")
                joinedCount++;
            else if (type == "interested")
                interestedCount++;
        }
    }

    pendingInteractionChecks--;

    if (pendingInteractionChecks <= 0)
    {
        ui->valueEventsJoined->setText(QString::number(joinedCount));
        ui->valueInterested->setText(QString::number(interestedCount));
    }
}
