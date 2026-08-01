#ifndef CREATEEVENTPAGE_H
#define CREATEEVENTPAGE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "Event.h"

namespace Ui {
class CreateEventPage;
}

class CreateEventPage : public QWidget
{
    Q_OBJECT

public:
    explicit CreateEventPage(QWidget *parent = nullptr);
    ~CreateEventPage();

    // Modes
    void newEvent();
    void editEvent(const Event &event);

signals:

    // Navigation
    void backRequested();
    void feedRequested();
    void notificationsRequested();
    void profileRequested();

    // Dashboard Refresh
    void eventCreated();
    void eventUpdated();

private slots:

    // Navigation
    void on_backButton_clicked();
    void on_navFeedButton_clicked();
    void on_navNotificationsButton_clicked();
    void on_navProfileButton_clicked();

    // Publish / Update
    void on_publishButton_clicked();

    // Network Reply
    void onReplyFinished(QNetworkReply *reply);

private:

    // Helpers
    void clearForm();

private:

    Ui::CreateEventPage *ui;

    QNetworkAccessManager *networkManager;

    //----------------------------------
    // Edit Mode
    //----------------------------------

    bool editMode = false;
    int editingEventId = -1;

    //----------------------------------
    // API
    //----------------------------------

    const QString BASE_URL =
        "http://127.0.0.1:18080";
};

#endif // CREATEEVENTPAGE_H