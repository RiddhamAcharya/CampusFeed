#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QList>

#include "Event.h"

class ApiService;

namespace Ui {
class DashBoardPage;
}

class DashBoardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashBoardPage(QWidget *parent = nullptr);
    ~DashBoardPage();

private slots:
    void displayEvents(QList<Event> events);
    void onNetworkError(QString message);

private:
    Ui::DashBoardPage *ui;
    ApiService *apiService;
};

#endif // DASHBOARDPAGE_H