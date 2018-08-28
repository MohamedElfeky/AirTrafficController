#include "mainwindow.h"
#include <QApplication>
#include <QObject>
#include <QString>
#include <QFile>
#include <QDir>
#include<QNetworkProxy>
#include "uav.h"
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QNetworkProxyFactory::setUseSystemConfiguration(true);
    QFile f(":/qdarkstyle/style.qss");
    qmlRegisterType<UAVNs::UAV>("UAV",1,0,"UAV");

    QDir atcDir;
    atcDir.setPath(QDir::homePath() + "/AirTrafficController");

    if(!atcDir.exists()){
        atcDir.mkpath(atcDir.path());
    }

    QFile fplan1(atcDir.path() + "/fplan1.txt");
    if(!fplan1.exists())
        QFile::copy(QString(qApp->applicationDirPath() + "/fplan1.txt"), QString(atcDir.path() + "/fplan1.txt"));

    QFile nofly(atcDir.path() + "/nofly.txt");
    if(!nofly.exists())
        QFile::copy(QString(qApp->applicationDirPath() + "/nofly.txt"), QString(atcDir.path() + "/nofly.txt"));

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
