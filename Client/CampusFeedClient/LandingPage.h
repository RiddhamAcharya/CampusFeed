#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QWidget>

namespace Ui {
class LandingPage;
}

class LandingPage : public QWidget
{
    Q_OBJECT

public:
    explicit LandingPage(QWidget *parent = nullptr);
    ~LandingPage();

signals:
    void loginRequested();
    void signupRequested();

private:
    Ui::LandingPage *ui;
};

#endif