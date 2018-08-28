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
    QVariantList generateRandomFlightPath(QGeoCoordinate home,int seed);
    Q_INVOKABLE void startSimulation();

    Q_INVOKABLE void startSimulationTraffic();

    Q_INVOKABLE bool checkLocnLiesinGeoFence(QGeoCoordinate coord);
    Q_INVOKABLE void sendUAVToLocn(QVariant coordvar);
    QTimer *timer = Q_NULLPTR;
    QList<QObject*> UAVList;
    QList<UAVNs::UAV*> uavListCpp;

    Q_PROPERTY(QString currentTestCase READ currentTestCase WRITE setCurrentTestCase NOTIFY currentTestCaseChanged)

    DroneRegistrationForm *regn = Q_NULLPTR;
    Q_INVOKABLE QVariantList readFlightPlanfromfile(QString filename);
    Q_INVOKABLE UAVNs::UAV * createUAV(int id, QGeoCoordinate home, QString iconAddr, QVariantList flpath );


    QMap<QTimer,int> timerList;
    QTimer *midAirTimer1 =Q_NULLPTR;
    QTimer *midAirTimer2 =Q_NULLPTR;
    QTimer *midAirTimer3 =Q_NULLPTR;
    QTimer *collTimer = Q_NULLPTR;
    QString currentTestCase() const
    {
        return m_currentTestCase;
    }

    bool geofenceBreached = false;
    bool collisionPossible = false;

    void checkIfDroneLiesWithinDia(float dia);
    void handleAvoidance(int i, int j);
public slots:
    void goToNextLocn();
    void goToNextLocnMidAir1();
    void goToNextLocnMidAir2();
    void goToNextLocnMidAir3();
    void restartDrone();
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

    void on_actionMid_Air_Collision_Avoidance_triggered();

private:
    Ui::MainWindow *ui;

    QString m_currentTestCase;
};

#endif // MAINWINDOW_H
