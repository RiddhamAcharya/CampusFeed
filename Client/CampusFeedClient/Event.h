#ifndef EVENT_H
#define EVENT_H

#include <QString>

struct Event
{
    int id;

    QString organizer;
    QString title;
    QString description;

    QString date;
    QString time;

    QString location;
    QString category;

    QString imagePath;

    QString registrationLink;

    QString timeAgo;

    QString eventDateTime;
};

#endif // EVENT_H