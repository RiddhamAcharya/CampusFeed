#ifndef NOTIFICATIONSPAGE_H
#define NOTIFICATIONSPAGE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>

namespace Ui {
class NotificationsPage;
}

class NotificationsPage : public QWidget
{
    Q_OBJECT

public:
    explicit NotificationsPage(QWidget *parent = nullptr);
    ~NotificationsPage();

    // Re-fetches notifications from the backend. Public so MainWindow
    // can call it every time this page is navigated to, keeping the
    // list fresh without any polling/timers.
    void fetchNotifications();

signals:
    void feedRequested();
    void profileRequested();

private slots:
    void on_navFeedButton_clicked();
    void on_navProfileButton_clicked();

    void onNotificationsReceived();
    void onMarkReadFinished();

    // Fired by a NotificationCard when the user clicks it.
    void onCardClicked(int notificationId);

private:
    void displayNotifications(const QJsonArray &array);
    void markAsRead(int notificationId);

private:
    Ui::NotificationsPage *ui;

    QNetworkAccessManager *networkManager;

    const QString BASE_URL = "http://127.0.0.1:18080";
};

#endif // NOTIFICATIONSPAGE_H
