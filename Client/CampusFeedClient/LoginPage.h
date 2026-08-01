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

    // Clears the email/password fields - called after logging out so a
    // stale password isn't left sitting in the form.
    void resetForm();

    // Clears the static session state (token/role/userId/fullName).
    static void logout();

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