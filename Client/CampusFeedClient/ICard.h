#ifndef ICARD_H
#define ICARD_H

#include <QString>

// Small abstract interface shared by the two "feed card" widgets in the
// app (EventCard, NotificationCard). It doesn't change how either widget
// already works - it's implemented alongside their existing QWidget base
// (multiple inheritance from one concrete class + one pure interface is
// safe here since ICard is not a QObject) purely to give both cards a
// common, polymorphic shape:
//
//   ICard *card = someEventCard;
//   card->refresh();       // works whether it's an EventCard or a
//                           // NotificationCard, without the caller
//                           // needing to know which.
//
// This is intentionally minimal - it demonstrates abstraction/
// inheritance/polymorphism without inventing a parallel architecture.
class ICard
{
public:
    virtual ~ICard() = default;

    // Re-synchronizes the card's displayed state with whatever backend
    // data it represents (EventCard re-checks the user's interaction;
    // NotificationCard has nothing further to re-fetch on its own, since
    // NotificationsPage owns the fetch/rebuild cycle for the whole list).
    virtual void refresh() = 0;

    // A short, human-readable label for what kind of card this is -
    // mostly useful for logging/debugging.
    virtual QString cardType() const = 0;
};

#endif // ICARD_H
