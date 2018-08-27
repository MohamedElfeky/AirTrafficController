#include "mainwindow.h"
#include <QApplication>
#include <QObject>
#include <QString>
#include <QFile>
#include <QDir>
#include<QNetworkProxy>
#include "uav.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QNetworkProxyFactory::setUseSystemConfiguration(true);
    QFile f(":/qdarkstyle/style.qss");
    qmlRegisterType<UAVNs::UAV>("UAV",1,0,"UAV");


    MainWindow w;
    w.show();
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
    return a.exec();
}