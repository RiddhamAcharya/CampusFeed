#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <QDateTime>
#include <QString>

// Small header-only helper for turning a backend timestamp into a
// human-friendly relative time string, e.g. "Just now", "5 minutes ago",
// "2 hours ago", "Yesterday", "3 days ago", "1 week ago".
//
// Kept as a free function (no class/state) so it can be reused from
// DashboardPage, EventCard and NotificationsPage without introducing
// any new architecture.
namespace TimeUtils
{
    inline QString relativeTime(const QString &backendDateTime)
    {
        if (backendDateTime.isEmpty())
            return "Recently";

        QDateTime dt = QDateTime::fromString(
            backendDateTime,
            "yyyy-MM-dd HH:mm:ss");

        if (!dt.isValid())
            return "Recently";

        QDateTime now = QDateTime::currentDateTime();

        qint64 secs = dt.secsTo(now);

        // Guard against clock skew / future timestamps
        if (secs < 0)
            secs = 0;

        if (secs < 60)
            return "Just now";

        qint64 minutes = secs / 60;
        if (minutes < 60)
            return QString("%1 minute%2 ago")
                .arg(minutes)
                .arg(minutes == 1 ? "" : "s");

        qint64 hours = minutes / 60;
        if (hours < 24)
            return QString("%1 hour%2 ago")
                .arg(hours)
                .arg(hours == 1 ? "" : "s");

        qint64 days = hours / 24;

        if (days == 1)
            return "Yesterday";

        if (days < 7)
            return QString("%1 days ago").arg(days);

        qint64 weeks = days / 7;
        if (weeks < 5)
            return QString("%1 week%2 ago")
                .arg(weeks)
                .arg(weeks == 1 ? "" : "s");

        qint64 months = days / 30;
        if (months < 12)
            return QString("%1 month%2 ago")
                .arg(months)
                .arg(months == 1 ? "" : "s");

        qint64 years = days / 365;
        return QString("%1 year%2 ago")
            .arg(years)
            .arg(years == 1 ? "" : "s");
    }

    // "Joined March 2025" style label, used on the Profile page.
    inline QString joinedLabel(const QString &backendDateTime)
    {
        QDateTime dt = QDateTime::fromString(
            backendDateTime,
            "yyyy-MM-dd HH:mm:ss");

        if (!dt.isValid())
            return "Joined recently";

        return "Joined " + dt.date().toString("MMMM yyyy");
    }
}

#endif // TIMEUTILS_H
