#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class ProfilePage;
}

class ProfilePage : public QWidget
{
    Q_OBJECT

public:
    explicit ProfilePage(QWidget *parent = nullptr);
    ~ProfilePage();

signals:
    void feedRequested();
    void notificationsRequested();

private slots:
    void on_navFeedButton_clicked();
    void on_navNotificationsButton_clicked();

private:
    Ui::ProfilePage *ui;
};

#endif