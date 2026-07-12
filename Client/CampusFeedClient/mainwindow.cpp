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
    // centralWidget()->setStyleSheet("background-color: #FFFFFF;");
    // ui->stackedWidget->setStyleSheet("background-color: #FFFFFF;");

    // ============================
    // Create Pages
    // ============================

    landingPage = new LandingPage(this);
    loginPage = new LoginPage(this);
    signupPage = new SignUpPage(this);
    dashboardPage = new DashBoardPage(this);

    // ============================
    // Add to Stacked Widget
    // ============================

    ui->stackedWidget->addWidget(landingPage);
    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->addWidget(signupPage);
    ui->stackedWidget->addWidget(dashboardPage);

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