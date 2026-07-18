#ifndef CREATEEVENTPAGE_H
#define CREATEEVENTPAGE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class CreateEventPage;
}

class CreateEventPage : public QWidget
{
    Q_OBJECT

public:
    explicit CreateEventPage(QWidget *parent = nullptr);
    ~CreateEventPage();

signals:
    // MainWindow Navigation
    void backRequested();
    void feedRequested();
    void notificationsRequested();
    void profileRequested();

    // Refresh Dashboard after successful publish
    void eventCreated();

private slots:

    // Navigation
    void on_backButton_clicked();
    void on_navFeedButton_clicked();
    void on_navNotificationsButton_clicked();
    void on_navProfileButton_clicked();

    // Event Creation
    void on_publishButton_clicked();

    // API Reply
    void onReplyFinished(QNetworkReply *reply);

private:

    Ui::CreateEventPage *ui;

    QNetworkAccessManager *networkManager;

    const QString BASE_URL =
        "http://127.0.0.1:18080";
};

#endif