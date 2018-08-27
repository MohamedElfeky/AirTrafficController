#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDesktopWidget>
#include <QQuickItem>
#include <QQuickWidget>
#include <QQmlContext>
#include <QFileDialog>
#include <QDir>
#include <QGeoCoordinate>
#include <QtAlgorithms>
#include <QStringList>
#include <QMessageBox>
#include <QActionGroup>
#include <QVariant>
#include <QVariantList>
#include <QApplication>
#include <QQuickView>
#include <QtMath>
#include "uav.h"
#include <QRectF>
#include<time.h>
#include <QTimer>
#include <cstdlib>
#include <ctime>
#include "random"
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showMap();
    QQuickView *mapView = Q_NULLPTR;
    QWidget *mapwidget = Q_NULLPTR;
    QQuickItem *flightmapitem = Q_NULLPTR;
    QDesktopWidget *desktop = QApplication::desktop();
    void setUpDroneEnv(int numDrones);
    QGeoCoordinate getRandomHomeLocn(qreal width, qreal height, qreal g , int seed);
    QVariantList generateRandomFlightPath(QGeoCoordinate home,int seed);
    Q_INVOKABLE void startSimulation();
    QTimer *timer = Q_NULLPTR;
    QList<QObject*> UAVList;
    QList<UAVNs::UAV*> uavListCpp;

//    std::mt19937 MyRNG;
//    uint32_t seed_val;
//    MyRNG rng;

public slots:
    void goToNextLocn();
private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
