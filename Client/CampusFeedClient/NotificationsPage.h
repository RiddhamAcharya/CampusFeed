#ifndef NOTIFICATIONSPAGE_H
#define NOTIFICATIONSPAGE_H

#include <QWidget>

namespace Ui {
class NotificationsPage;
}

class NotificationsPage : public QWidget
{
    Q_OBJECT

public:
    explicit NotificationsPage(QWidget *parent = nullptr);
    ~NotificationsPage();

signals:
    void feedRequested();
    void profileRequested();

private slots:
    void on_navFeedButton_clicked();
    void on_navProfileButton_clicked();

private:
    Ui::NotificationsPage *ui;
};

#endif