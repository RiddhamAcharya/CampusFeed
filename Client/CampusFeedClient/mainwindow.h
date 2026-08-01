#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "LandingPage.h"
#include "LoginPage.h"
#include "SignupPage.h"
#include "DashboardPage.h"
#include "NotificationsPage.h"
#include "ProfilePage.h"
#include "CreateEventPage.h"
#include "Event.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openEditEvent(const Event &event);

private:
    Ui::MainWindow *ui;

    LandingPage *landingPage;
    LoginPage *loginPage;
    SignUpPage *signupPage;
    DashBoardPage *dashboardPage;

    NotificationsPage *notificationsPage;
    ProfilePage *profilePage;
    CreateEventPage *createEventPage;
};

#endif // MAINWINDOW_H