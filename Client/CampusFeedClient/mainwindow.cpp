#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (centralWidget()->layout()) {
        centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
        centralWidget()->layout()->setSpacing(0);
    }

    // Safety-net background: each page already paints its own white
    // background (see WA_StyledBackground fix in DashboardPage etc.),
    // but this gives every page a white fallback at the container level
    // too, in case a future page/frame is added without its own
    // background set.
    centralWidget()->setStyleSheet("background-color: #FFFFFF;");
    ui->stackedWidget->setStyleSheet("background-color: #FFFFFF;");

    // ============================
    // Create Pages
    // ============================

    landingPage = new LandingPage(this);
    loginPage = new LoginPage(this);
    signupPage = new SignUpPage(this);
    dashboardPage = new DashBoardPage(this);

    notificationsPage = new NotificationsPage(this);
    profilePage = new ProfilePage(this);
    createEventPage = new CreateEventPage(this);

    connect(dashboardPage,
            &DashBoardPage::notificationsRequested,
            this,
            [=]()
            {
                notificationsPage->fetchNotifications();
                ui->stackedWidget->setCurrentWidget(notificationsPage);
            });

    connect(dashboardPage,
            &DashBoardPage::profileRequested,
            this,
            [=]()
            {
                profilePage->refreshProfile();
                ui->stackedWidget->setCurrentWidget(profilePage);
            });
    connect(notificationsPage,
            &NotificationsPage::feedRequested,
            this,
            [=]()
            {
                dashboardPage->fetchEvents();
                ui->stackedWidget->setCurrentWidget(dashboardPage);
            });

    connect(notificationsPage,
            &NotificationsPage::profileRequested,
            this,
            [=]()
            {
                profilePage->refreshProfile();
                ui->stackedWidget->setCurrentWidget(profilePage);
            });
    connect(profilePage,
            &ProfilePage::feedRequested,
            this,
            [=]()
            {
                dashboardPage->fetchEvents();
                ui->stackedWidget->setCurrentWidget(dashboardPage);
            });

    connect(profilePage,
            &ProfilePage::notificationsRequested,
            this,
            [=]()
            {
                notificationsPage->fetchNotifications();
                ui->stackedWidget->setCurrentWidget(notificationsPage);
            });

    // ============================
    // Logout
    // ============================

    connect(profilePage,
            &ProfilePage::logoutRequested,
            this,
            [=]()
            {
                LoginPage::logout();
                loginPage->resetForm();

                ui->stackedWidget->setCurrentWidget(loginPage);
            });

    connect(dashboardPage,
            &DashBoardPage::createEventRequested,
            this,
            [=]()
            {
                createEventPage->newEvent();
                ui->stackedWidget->setCurrentWidget(createEventPage);
            });

    connect(dashboardPage,
            &DashBoardPage::editEventRequested,
            this,
            &MainWindow::openEditEvent);

    connect(createEventPage,
            &CreateEventPage::backRequested,
            this,
            [=]()
            {
                ui->stackedWidget->setCurrentWidget(dashboardPage);
            });

    connect(createEventPage,
            &CreateEventPage::eventCreated,
            dashboardPage,
            &DashBoardPage::fetchEvents);

    // Phase 1 fix: editing an event previously never refreshed the
    // feed (only eventCreated was connected), so an edit looked like
    // it silently did nothing until the next manual refresh.
    connect(createEventPage,
            &CreateEventPage::eventUpdated,
            dashboardPage,
            &DashBoardPage::fetchEvents);

    connect(createEventPage,
            &CreateEventPage::feedRequested,
            this,
            [=]()
            {
                dashboardPage->fetchEvents();
                ui->stackedWidget->setCurrentWidget(dashboardPage);
            });

    connect(createEventPage,
            &CreateEventPage::notificationsRequested,
            this,
            [=]()
            {
                notificationsPage->fetchNotifications();
                ui->stackedWidget->setCurrentWidget(notificationsPage);
            });

    connect(createEventPage,
            &CreateEventPage::profileRequested,
            this,
            [=]()
            {
                profilePage->refreshProfile();
                ui->stackedWidget->setCurrentWidget(profilePage);
            });


    // ============================
    // Add to Stacked Widget
    // ============================

    ui->stackedWidget->addWidget(landingPage);
    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->addWidget(signupPage);
    ui->stackedWidget->addWidget(dashboardPage);
    ui->stackedWidget->addWidget(notificationsPage);
    ui->stackedWidget->addWidget(profilePage);
    ui->stackedWidget->addWidget(createEventPage);

    // ============================
    // Landing Page Navigation
    // ============================

    connect(landingPage,
            &LandingPage::loginRequested,
            this,
            [=]()
            {
                ui->stackedWidget->setCurrentWidget(loginPage);
            });

    connect(landingPage,
            &LandingPage::signupRequested,
            this,
            [=]()
            {
                ui->stackedWidget->setCurrentWidget(signupPage);
            });

    // ============================
    // Login Page Navigation
    // ============================

    connect(loginPage,
            &LoginPage::signupRequested,
            this,
            [=]()
            {
                ui->stackedWidget->setCurrentWidget(signupPage);
            });

    connect(loginPage,
            &LoginPage::loginSuccessful,
            this,
            [=]()
            {
                dashboardPage->fetchEvents();
                ui->stackedWidget->setCurrentWidget(dashboardPage);
            });

    // ============================
    // Signup Page Navigation
    // ============================

    connect(signupPage,
            &SignUpPage::loginRequested,
            this,
            [=]()
            {
                ui->stackedWidget->setCurrentWidget(loginPage);
            });

    connect(signupPage,
            &SignUpPage::signupSuccessful,
            this,
            [=]()
            {
                ui->stackedWidget->setCurrentWidget(loginPage);
            });

    // ============================
    // Start Application
    // ============================

    ui->stackedWidget->setCurrentWidget(landingPage);
}
void MainWindow::openEditEvent(const Event &event)
{
    createEventPage->editEvent(event);

    ui->stackedWidget->setCurrentWidget(createEventPage);
}
MainWindow::~MainWindow()
{
    delete ui;
}
