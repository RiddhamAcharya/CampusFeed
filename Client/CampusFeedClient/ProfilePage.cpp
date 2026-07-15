#include "ProfilePage.h"
#include "ui_ProfilePage.h"

ProfilePage::ProfilePage(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::ProfilePage)
{
    ui->setupUi(this);

    connect(ui->navFeedButton,
            &QPushButton::clicked,
            this,
            &ProfilePage::on_navFeedButton_clicked);

    connect(ui->navNotificationsButton,
            &QPushButton::clicked,
            this,
            &ProfilePage::on_navNotificationsButton_clicked);
}

ProfilePage::~ProfilePage()
{
    delete ui;
}

void ProfilePage::on_navFeedButton_clicked()
{
    emit feedRequested();
}

void ProfilePage::on_navNotificationsButton_clicked()
{
    emit notificationsRequested();
}