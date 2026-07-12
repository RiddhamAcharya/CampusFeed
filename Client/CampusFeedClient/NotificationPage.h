#ifndef NOTIFICATIONPAGE_H
#define NOTIFICATIONPAGE_H

#include <QWidget>

namespace Ui {
class NotificationPage;
}

class NotificationPage : public QWidget
{
    Q_OBJECT

public:
    explicit NotificationPage(QWidget *parent = nullptr);
    ~NotificationPage();

private:
    Ui::NotificationPage *ui;
};

#endif // NOTIFICATIONPAGE_H
