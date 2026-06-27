#include "DashboardPage.h"
#include "ui_DashboardPage.h"

DashBoardPage::DashBoardPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DashBoardPage)
{
    ui->setupUi(this);
}

DashBoardPage::~DashBoardPage()
{
    delete ui;
}
