#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyleSheet(R"(
QMessageBox QLabel {
    color: black;
}

QMessageBox QPushButton {
    color: black;
    background-color: white;
    border: 1px solid #D1D5DB;
    border-radius: 6px;
    padding: 6px 14px;
}

QMessageBox QPushButton:hover {
    background-color: #F3F4F6;
}
)");

    MainWindow w;
    w.show();

    return a.exec();
}