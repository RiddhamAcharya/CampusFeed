#ifndef EVENT_H
#define EVENT_H

#include <QString>

struct Event
{
    int id;

    // Id of the user who created this event (used for ownership checks,
    // e.g. only showing the Edit/Delete menu to the event's own creator).
    int organizerId = -1;

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

    // Raw "created_at" timestamp from the backend (yyyy-MM-dd HH:mm:ss),
    // used to sort the feed newest-first and to compute timeAgo.
    QString createdAt;
};

#endif // EVENT_H