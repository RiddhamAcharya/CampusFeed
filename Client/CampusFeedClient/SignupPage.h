#ifndef SIGNUPPAGE_H
#define SIGNUPPAGE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class SignUpPage;
}

class SignUpPage : public QWidget
{
    Q_OBJECT

public:
    explicit SignUpPage(QWidget *parent = nullptr);
    ~SignUpPage();

signals:
    void signupSuccessful();
    void loginRequested();

private slots:
    void onSignupClicked();
    void onReplyFinished(QNetworkReply *reply);
    void onBrowseClicked();

private:
    Ui::SignUpPage *ui;

    QNetworkAccessManager *networkManager;

    QString selectedRole = "student";
    QString profileImagePath;
};

#endif