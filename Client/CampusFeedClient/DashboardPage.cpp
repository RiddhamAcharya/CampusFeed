#include "DashboardPage.h"
#include "ui_DashboardPage.h"

#include "EventCard.h"
#include "Event.h"

#include <QList>

DashBoardPage::DashBoardPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DashBoardPage)
{
    ui->setupUi(this);

    loadDummyEvents();
}

DashBoardPage::~DashBoardPage()
{
    delete ui;
}

void DashBoardPage::loadDummyEvents()
{
    QList<Event> events;

    Event event1;
    event1.organizer = "Computer Science Society";
    event1.title = "Workshop on Web Development using Qt & C++";
    event1.description = "Join us this Saturday in Lab 3. Open to all students interested in Qt and C++.";
    event1.date = "28 June 2026";
    event1.time = "11:00 AM";
    event1.location = "Lab 3";
    event1.category = "Workshop";
    event1.timeAgo = "2 hours ago";
    events.append(event1);

    Event event2;
    event2.organizer = "IEEE Student Branch";
    event2.title = "Hackathon 2026 Registration Open";
    event2.description = "Register your team and compete for exciting prizes.";
    event2.date = "5 July 2026";
    event2.time = "9:00 AM";
    event2.location = "Innovation Center";
    event2.category = "Competition";
    event2.timeAgo = "5 hours ago";
    events.append(event2);

    Event event3;
    event3.organizer = "Photography Club";
    event3.title = "Campus Photo Walk";
    event3.description = "Bring your camera and explore the campus together.";
    event3.date = "10 July 2026";
    event3.time = "3:00 PM";
    event3.location = "Main Gate";
    event3.category = "Activity";
    event3.timeAgo = "1 day ago";
    events.append(event3);

    for (const Event &event : events)
    {
        EventCard *card = new EventCard(this);
        card->setEvent(event);

        ui->feedLayout->addWidget(card);
    }

    ui->feedLayout->addStretch();
}