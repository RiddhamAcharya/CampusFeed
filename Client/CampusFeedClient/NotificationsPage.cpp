#include "NotificationsPage.h"
#include "ui_NotificationsPage.h"

NotificationsPage::NotificationsPage(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::NotificationsPage)
{
    ui->setupUi(this);

    connect(ui->navFeedButton,
            &QPushButton::clicked,
            this,
            &NotificationsPage::on_navFeedButton_clicked);

    connect(ui->navProfileButton,
            &QPushButton::clicked,
            this,
            &NotificationsPage::on_navProfileButton_clicked);
}

NotificationsPage::~NotificationsPage()
{
    delete ui;
}

void NotificationsPage::on_navFeedButton_clicked()
{
    emit feedRequested();
}

void NotificationsPage::on_navProfileButton_clicked()
{
    emit profileRequested();
}