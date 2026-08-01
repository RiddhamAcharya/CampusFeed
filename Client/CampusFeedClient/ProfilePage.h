#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class ProfilePage;
}

class ProfilePage : public QWidget
{
    Q_OBJECT

public:
    explicit ProfilePage(QWidget *parent = nullptr);
    ~ProfilePage();

    // Re-fetches profile info + stats from the backend. Public so
    // MainWindow can call it every time this page is navigated to.
    void refreshProfile();

signals:
    void feedRequested();
    void notificationsRequested();

    // Emitted after the user confirms logging out - MainWindow handles
    // clearing session state and returning to the login screen.
    void logoutRequested();

private slots:
    void on_navFeedButton_clicked();
    void on_navNotificationsButton_clicked();
    void on_logoutButton_clicked();

    void onProfileReplyFinished();
    void onMyEventsReplyFinished();
    void onAllEventsForStatsReceived();
    void onInteractionCheckReceived();

private:
    void applyLocalSessionInfo();
    void fetchMyEventsCount();
    void fetchInteractionStats();

private:
    Ui::ProfilePage *ui;

    QNetworkAccessManager *networkManager;

    // Running counters used while tallying "Events Joined" / "Interested"
    // across every event (there's no single aggregate endpoint for this,
    // so we check each event's interaction for the current user).
    int pendingInteractionChecks = 0;
    int joinedCount = 0;
    int interestedCount = 0;

    const QString BASE_URL = "http://127.0.0.1:18080";
};

#endif // PROFILEPAGE_H
