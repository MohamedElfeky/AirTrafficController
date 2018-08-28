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
#include "droneregistrationform.h"
#include <QFile>
#include <QTextStream>

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
    QQuickView *atcView = Q_NULLPTR;
    QWidget *mapwidget = Q_NULLPTR;
    QQuickItem *flightmapitem = Q_NULLPTR;
    QDesktopWidget *desktop = QApplication::desktop();
    void setUpDroneEnv(int numDrones);
    QList<QGeoCoordinate> getRandomHomeLocn(qreal width, qreal height, qreal g , int seed, int numDrones);
    QVariantList generateRandomFlightPath(QGeoCoordinate home,int seed);
    Q_INVOKABLE void startSimulation();

    Q_INVOKABLE bool checkLocnLiesinGeoFence(QGeoCoordinate coord);
    Q_INVOKABLE void sendUAVToLocn(QVariant coordvar);
    QTimer *timer = Q_NULLPTR;
    QList<QObject*> UAVList;
    QList<UAVNs::UAV*> uavListCpp;

    Q_PROPERTY(QString currentTestCase READ currentTestCase WRITE setCurrentTestCase NOTIFY currentTestCaseChanged)

    DroneRegistrationForm *regn = Q_NULLPTR;
    Q_INVOKABLE QVariantList readFlightPlanfromfile(QString filename);
//    std::mt19937 MyRNG;
//    uint32_t seed_val;
//    MyRNG rng;

    QString currentTestCase() const
    {
        return m_currentTestCase;
    }

    bool geofenceBreached = false;
public slots:
    void goToNextLocn();
    void setCurrentTestCase(QString currentTestCase)
    {
        if (m_currentTestCase == currentTestCase)
            return;

        m_currentTestCase = currentTestCase;
        emit currentTestCaseChanged(m_currentTestCase);
    }

signals:
    void currentTestCaseChanged(QString currentTestCase);

private slots:
    void on_actionGeoFence_Breach_triggered();

    void on_actionDrone_Registration_triggered();

    void onDroneRegnFormSubmitted();

    void on_actionNo_Fly_Zone_triggered();

private:
    Ui::MainWindow *ui;

    QString m_currentTestCase;
};

#endif // MAINWINDOW_H
