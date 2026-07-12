#include "NotificationCard.h"
#include "ui_NotificationCard.h"

NotificationCard::NotificationCard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NotificationCard)
{
    ui->setupUi(this);
}

NotificationCard::~NotificationCard()
{
    delete ui;
}
