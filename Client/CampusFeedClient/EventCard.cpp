#include "EventCard.h"
#include "ui_EventCard.h"
#include "Event.h"

#include <QPixmap>

EventCard::EventCard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EventCard)
{
    ui->setupUi(this);

    // Make description wrap nicely
    ui->descriptionLabel->setWordWrap(true);

    // Make the image scale properly
    ui->eventImageLabel->setScaledContents(true);
}

EventCard::~EventCard()
{
    delete ui;
}

void EventCard::setEvent(const Event &event)
{
    // Header
    ui->organizerLabel->setText(event.organizer);
    ui->titleLabel->setText(event.title);
    ui->descriptionLabel->setText(event.description);

    // Details Row
    ui->dateLabel->setText(event.date);
    ui->eventTimeLabel->setText(event.time);
    ui->locationLabel->setText(event.location);
    ui->categoryLabel->setText(event.category);

    // Event Image
    if (!event.imagePath.isEmpty())
    {
        QPixmap pixmap(event.imagePath);

        if (!pixmap.isNull())
        {
            ui->eventImageLabel->setPixmap(
                pixmap.scaled(
                    ui->eventImageLabel->size(),
                    Qt::KeepAspectRatioByExpanding,
                    Qt::SmoothTransformation));
        }
    }

    // Reset button states
    ui->interestedButton->setChecked(false);
    ui->notInterestedButton->setChecked(false);
    ui->goingButton->setChecked(false);
}