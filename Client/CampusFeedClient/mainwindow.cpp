#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create pages
    loginPage = new LoginPage(this);

    connect(loginPage,
            &LoginPage::loginSuccessful,
            this,
            [this]()
            {
                ui->stackedWidget->setCurrentWidget(dashboardPage);
            });
    connect(loginPage,
            &LoginPage::signupRequested,
            this,
            [this]()
            {
                ui->stackedWidget->setCurrentWidget(signupPage);
            });

    signupPage = new SignUpPage(this);

    connect(signupPage,
            &SignUpPage::loginRequested,
            this,
            [this]()
            {
                ui->stackedWidget->setCurrentWidget(loginPage);
            });

    connect(signupPage,
            &SignUpPage::signupSuccessful,
            this,
            [this]()
            {
                ui->stackedWidget->setCurrentWidget(loginPage);
            });

    dashboardPage = new DashBoardPage(this);

    // Add pages to stacked widget
    ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->addWidget(signupPage);
    ui->stackedWidget->addWidget(dashboardPage);

    // Show login page first
    ui->stackedWidget->setCurrentWidget(loginPage);
}

MainWindow::~MainWindow()
{
    delete ui;
}