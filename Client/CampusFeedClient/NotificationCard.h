#ifndef NOTIFICATIONCARD_H
#define NOTIFICATIONCARD_H

#include <QWidget>
#include <QString>

#include "ICard.h"

namespace Ui {
class NotificationCard;
}

class NotificationCard : public QWidget, public ICard
{
    Q_OBJECT

public:
    explicit NotificationCard(QWidget *parent = nullptr);
    ~NotificationCard();

    // Populates the card and computes a relative timestamp from
    // createdAt (backend "yyyy-MM-dd HH:mm:ss" format).
    void setNotification(int id,
                          const QString &title,
                          const QString &message,
                          const QString &createdAt,
                          bool isRead);

    // ICard
    void refresh() override;
    QString cardType() const override;

signals:
    // Emitted when the user clicks anywhere on the card - used by
    // NotificationsPage to mark it as read.
    void clicked(int notificationId);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::NotificationCard *ui;

    int notificationId = -1;
};

#endif // NOTIFICATIONCARD_H
