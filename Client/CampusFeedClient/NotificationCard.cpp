#include "NotificationCard.h"
#include "ui_NotificationCard.h"
#include "TimeUtils.h"

#include <QMouseEvent>

NotificationCard::NotificationCard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NotificationCard)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground, true);
    setCursor(Qt::PointingHandCursor);

    // No delete/manage backend route exists for notifications yet -
    // hide the placeholder menu button rather than ship a dead control.
    ui->menuButton->setVisible(false);
}

NotificationCard::~NotificationCard()
{
    delete ui;
}

void NotificationCard::setNotification(int id,
                                        const QString &title,
                                        const QString &message,
                                        const QString &createdAt,
                                        bool isRead)
{
    notificationId = id;

    ui->organizationLabel->setText(title);
    ui->messageLabel->setText(message);
    ui->timeLabel->setText(TimeUtils::relativeTime(createdAt));

    ui->unreadIndicator->setVisible(!isRead);

    // Slightly mute already-read notifications so unread ones stand out,
    // without changing the overall card design.
    if (isRead)
    {
        ui->organizationLabel->setStyleSheet("color: #6B7280;");
        setStyleSheet(
            "QWidget#NotificationCard {"
            "    background-color: #FBFBFD;"
            "    border: 1px solid #E6EAF0;"
            "    border-radius: 10px;"
            "}");
    }
    else
    {
        ui->organizationLabel->setStyleSheet("color: #1F2937;");
        setStyleSheet(
            "QWidget#NotificationCard {"
            "    background-color: #FFFFFF;"
            "    border: 1px solid #E6EAF0;"
            "    border-radius: 10px;"
            "}"
            "QWidget#NotificationCard:hover {"
            "    background-color: #F7F9FC;"
            "}");
    }
}

void NotificationCard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked(notificationId);
    }

    QWidget::mousePressEvent(event);
}

void NotificationCard::refresh()
{
    // The list itself is rebuilt by NotificationsPage on every fetch,
    // so there's nothing per-card to re-sync here - present for a
    // consistent ICard interface across both card types.
}

QString NotificationCard::cardType() const
{
    return "Notification";
}
