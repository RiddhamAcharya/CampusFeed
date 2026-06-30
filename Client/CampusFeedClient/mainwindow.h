#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "LoginPage.h"
#include "SignupPage.h"
#include "DashboardPage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    LoginPage *loginPage;
    SignUpPage *signupPage;
    DashBoardPage *dashboardPage;
};

#endif // MAINWINDOW_H