#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // if (centralWidget()->layout()) {
    //     centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
    //     centralWidget()->layout()->setSpacing(0);
    // }
    // centralWidget()->setStyleSheet("background-color: #FFFFFF;");
    // ui->stackedWidget->setStyleSheet("background-color: #FFFFFF;");

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
                ui->stackedWidget->setCurrentWidget(notificationsPage);
            });

    connect(dashboardPage,
            &DashBoardPage::profileRequested,
            this,
            [=]()
            {
                ui->stackedWidget->setCurrentWidget(profilePage);
            });
    connect(notificationsPage,
            &NotificationsPage::feedRequested,
            this,
            [=]()
            {
                ui->stackedWidget->setCurrentWidget(dashboardPage);
            });

    connect(notificationsPage,
            &NotificationsPage::profileRequested,
            this,
            [=]()
            {
                ui->stackedWidget->setCurrentWidget(profilePage);
            });
    connect(profilePage,
            &ProfilePage::feedRequested,
            this,
            [=]()
            {
                ui->stackedWidget->setCurrentWidget(dashboardPage);
            });

    connect(profilePage,
            &ProfilePage::notificationsRequested,
            this,
            [=]()
            {
                ui->stackedWidget->setCurrentWidget(notificationsPage);
            });
    connect(dashboardPage,
            &DashBoardPage::createEventRequested,
            this,
            [=]()
            {
                ui->stackedWidget->setCurrentWidget(createEventPage);
            });
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

    connect(createEventPage,
            &CreateEventPage::feedRequested,
            this,
            [=]()
            {
                ui->stackedWidget->setCurrentWidget(dashboardPage);
            });

    connect(createEventPage,
            &CreateEventPage::notificationsRequested,
            this,
            [=]()
            {
                ui->stackedWidget->setCurrentWidget(notificationsPage);
            });

    connect(createEventPage,
            &CreateEventPage::profileRequested,
            this,
            [=]()
            {
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

MainWindow::~MainWindow()
{
    delete ui;
}