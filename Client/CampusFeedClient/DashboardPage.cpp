#include "DashboardPage.h"
#include "ui_DashboardPage.h"

#include "ApiService.h"
#include "EventCard.h"

#include <QMessageBox>

DashBoardPage::DashBoardPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DashBoardPage)
{
    ui->setupUi(this);

    apiService = new ApiService(this);

    connect(apiService,
            &ApiService::eventsReceived,
            this,
            &DashBoardPage::displayEvents);

    connect(apiService,
            &ApiService::networkError,
            this,
            &DashBoardPage::onNetworkError);

    apiService->fetchEvents();
}

DashBoardPage::~DashBoardPage()
{
    delete ui;
}

void DashBoardPage::displayEvents(QList<Event> events)
{
    // Remove old cards
    QLayoutItem *item;

    while ((item = ui->feedLayout->takeAt(0)) != nullptr)
    {
        if (item->widget())
            delete item->widget();

        delete item;
    }

    // Add new cards
    for (const Event &event : events)
    {
        EventCard *card = new EventCard(this);
        card->setEvent(event);

        ui->feedLayout->addWidget(card);
    }

    ui->feedLayout->addStretch();
}

void DashBoardPage::onNetworkError(QString message)
{
    QMessageBox::warning(this,
                         "Network Error",
                         message);
}