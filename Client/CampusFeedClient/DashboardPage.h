#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>

namespace Ui {
class DashBoardPage;
}

class DashBoardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashBoardPage(QWidget *parent = nullptr);
    ~DashBoardPage();

private:
    Ui::DashBoardPage *ui;
};

#endif // DASHBOARDPAGE_H
