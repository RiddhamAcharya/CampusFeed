#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create pages
    loginPage = new LoginPage(this);
    signupPage = new SignUpPage(this);
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