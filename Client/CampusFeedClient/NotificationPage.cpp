#include "NotificationPage.h"
#include "ui_NotificationPage.h"

NotificationPage::NotificationPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NotificationPage)
{
    ui->setupUi(this);
}

NotificationPage::~NotificationPage()
{
    delete ui;
}
