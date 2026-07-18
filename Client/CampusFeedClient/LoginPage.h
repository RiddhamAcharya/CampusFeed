#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();
    static QString token;
    static QString role;
    static int userId;
    static QString fullName;

signals:
    void loginSuccessful();
    void signupRequested();

private slots:
    void onLoginClicked();
    void onReplyFinished(QNetworkReply *reply);

private:
    Ui::LoginPage *ui;
    QNetworkAccessManager *networkManager;
};

#endif