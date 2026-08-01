#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Global stylesheet (QMessageBox, QMenu, QToolTip) lives in
    // Assets/styles/app.qss and is bundled via resources.qrc, instead
    // of being an inline raw string here.
    QFile styleFile(":/Assets/styles/app.qss");

    if (styleFile.open(QFile::ReadOnly | QFile::Text))
    {
        a.setStyleSheet(QString::fromUtf8(styleFile.readAll()));
        styleFile.close();
    }
    else
    {
        qWarning() << "Could not load app.qss - falling back to default styling.";
    }

    MainWindow w;
    w.show();

    return a.exec();
}
