#include "LandingPage.h"
#include "ui_LandingPage.h"

LandingPage::LandingPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LandingPage)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground, true);

    connect(ui->loginButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                emit loginRequested();
            });

    connect(ui->signupButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                emit signupRequested();
            });
}

LandingPage::~LandingPage()
{
    delete ui;
}