#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionDrone_Registration->setVisible(false);
    setWindowTitle("Air Traffic Controller");
    showMap();
    setUpDroneEnv(5);

}

MainWindow::~MainWindow()
{
    delete ui;
    if(regn != Q_NULLPTR){
        delete regn;
    }
}

void MainWindow::showMap(){

    atcView = new QQuickView;
    atcView->setSource(QUrl(QStringLiteral("qrc:/qml_maps/FlightMapViewer.qml")));

    atcView->setResizeMode(QQuickView::SizeRootObjectToView);

    if (atcView->status() == QQuickView::Error) {
        qWarning() << atcView->errors();
        return;
    }
    mapwidget = QWidget::createWindowContainer(atcView, this);
    ui->maingridLayout->addWidget(mapwidget);
    setFocusProxy(mapwidget); // focus container widget when top level widget is focused
    //resize(size);
    setFocusPolicy(Qt::NoFocus);

    atcView->rootObject()->setProperty("width",this->width());
    atcView->rootObject()->setProperty("height",this->height());
    flightmapitem = atcView->rootObject()->findChild<QQuickItem*>("map_main");
    atcView->rootContext()->setContextProperty("_controlWindow",this);

    //    addMapTypes();
    //    connect(this,SIGNAL(MapTypeChanged(QVariant)),flightmapitem,SLOT(changeMapType(QVariant)));

    QMainWindow::showMaximized();
    QApplication::processEvents();

}

void MainWindow::setUpDroneEnv(int numDrones)
{

    QList<QGeoCoordinate> homeLocns = getRandomHomeLocn(1300,800,numDrones+10,50,5);
}

QList<QGeoCoordinate> MainWindow::getRandomHomeLocn(qreal width, qreal height, qreal g, int seed, int numDrones)
{

}

QVariantList MainWindow::generateRandomFlightPath(QGeoCoordinate home,int seed)
{
    if(uavListCpp.length() == 0){
        srand(seed);
        qreal bng = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/360));
        qreal dist = 50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1000));
        //        qDebug() << bng << dist <<"flightpath";
        QGeoCoordinate nxtpt = home.atDistanceAndAzimuth(dist,bng);
        QVariantList flPath;
        home.setAltitude(130);
        nxtpt.setAltitude(130);
        flPath.push_back(QVariant::fromValue(home));
        flPath.push_back(QVariant::fromValue(nxtpt));

        bng = bng +90;
        dist = 30;
        nxtpt = nxtpt.atDistanceAndAzimuth(dist,bng);
        flPath.push_back(QVariant::fromValue(nxtpt));

        bng = bng +90;
        dist = 50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/250));
        nxtpt = nxtpt.atDistanceAndAzimuth(dist,bng);
        flPath.push_back(QVariant::fromValue(nxtpt));

        bng = bng +static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/180));
        dist = 50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/250));
        nxtpt = nxtpt.atDistanceAndAzimuth(dist,bng);
        flPath.push_back(QVariant::fromValue(nxtpt));
        return flPath;
    }
    else if(uavListCpp.length() == 1){
        srand(seed);
        qreal bng = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/360));
        qreal dist = 50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1500));
        QGeoCoordinate nxtpt = home.atDistanceAndAzimuth(dist,bng);
        QVariantList flPath;
        home.setAltitude(130);
        nxtpt.setAltitude(130);
        flPath.push_back(QVariant::fromValue(home));
        flPath.push_back(QVariant::fromValue(nxtpt));
        return flPath;
    }
    else if(uavListCpp.length() == 2){
        srand(seed);
        qreal bng = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/90));
        qreal dist = 50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1000));
        QGeoCoordinate nxtpt = home.atDistanceAndAzimuth(dist,bng);
        QVariantList flPath;
        home.setAltitude(130);
        nxtpt.setAltitude(130);

        bng = bng +90;
        dist = 30;
        nxtpt = nxtpt.atDistanceAndAzimuth(dist,bng);
        flPath.push_back(QVariant::fromValue(nxtpt));

        srand(seed + 15);
        bng = bng +90;
        dist = 50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/250));
        nxtpt = nxtpt.atDistanceAndAzimuth(dist,bng);
        flPath.push_back(QVariant::fromValue(nxtpt));

        bng = bng +90;
        dist = 30;
        nxtpt = nxtpt.atDistanceAndAzimuth(dist,bng);
        flPath.push_back(QVariant::fromValue(nxtpt));

        srand(seed + time(0));
        bng = bng +90;
        dist = 50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/250));
        nxtpt = nxtpt.atDistanceAndAzimuth(dist,bng);
        flPath.push_back(QVariant::fromValue(nxtpt));

        return flPath;

    }


}

void MainWindow::startSimulation()
{
    qDebug() << uavListCpp[0]->currentLocn() << "startSimulation";
    uavListCpp[0]->currentLocn().setAltitude(130);

    if(timer == Q_NULLPTR){
        timer = new QTimer(this);
    }
    connect(timer,SIGNAL(timeout()),this,SLOT(goToNextLocn()));
    timer->start(1000);


}



void MainWindow::goToNextLocn()
{

    //        qDebug() << uavListCpp[0]->currentLocn().distanceTo(uavListCpp[0]->nextLocn()) <<  "goToNextLocn";

    //    if(uavListCpp[0]->nextLocn().altitude() -uavListCpp[0]->currentLocn().altitude() > 10  ){
    //        qDebug() << uavListCpp[0]->currentLocn().altitude();
    //        double alt = 10 + uavListCpp[0]->currentLocn().altitude();
    //        uavListCpp[0]->currentLocn().setAltitude(alt);
    //        uavListCpp[0]->nextLocn().setAltitude(alt+10);
    //        QMetaObject::invokeMethod(flightmapitem,"vehDataChanged",Q_ARG(QVariant,qVariantFromValue(uavListCpp[0]->currentLocn())));

    //    }
    /*else*/ if(uavListCpp[0]->currentLocn().distanceTo(uavListCpp[0]->nextLocn()) > 10){
        QGeoCoordinate nLocn;
        double dist = 10;
        double azim = uavListCpp[0]->currentLocn().azimuthTo(uavListCpp[0]->nextLocn());
        nLocn = uavListCpp[0]->currentLocn().atDistanceAndAzimuth(dist,azim);
        uavListCpp[0]->setCurrentLocn(nLocn);
        QMetaObject::invokeMethod(flightmapitem,"vehDataChanged",Q_ARG(QVariant,qVariantFromValue(nLocn)));
        if(nLocn.distanceTo(uavListCpp[0]->destnLocn()) < 10){
            timer->stop();
            qDebug() << "reached destn";
            if(currentTestCase() =="GeoFence Breach" ){
                QMetaObject::invokeMethod(flightmapitem,"displayGeoFenceAfterBreach");
            }
        }
    }
    else{
        qDebug() << "reached" << uavListCpp[0]->indexReached();

        if(uavListCpp[0]->indexReached() +1 < uavListCpp[0]->flightPath().length()){

            uavListCpp[0]->setNextLocn(uavListCpp[0]->flightPath()[uavListCpp[0]->indexReached() +1].value<QGeoCoordinate>());
            uavListCpp[0]->setIndexReached(uavListCpp[0]->indexReached() +1);
        }
        else{
            timer->stop();
            qDebug() << "reached destn";
        }

    }

    if(currentTestCase() == "GeoFence Breach"){
        if(!checkLocnLiesinGeoFence(uavListCpp[0]->currentLocn()) && !geofenceBreached)
        {
            qDebug() << "geoFence Breach";
            QMetaObject::invokeMethod(flightmapitem,"displayGeoFenceBreach");
            geofenceBreached = true;
        }
    }
}


bool MainWindow::checkLocnLiesinGeoFence(QGeoCoordinate coord)
{

    QGeoCoordinate circleCenter;
    circleCenter = uavListCpp[0]->homeLocn().value<QGeoCoordinate>();

    double radius = 500;
    double dist = circleCenter.distanceTo(coord);
    if(dist < radius)
        return true;
    return false;
}

void MainWindow::sendUAVToLocn(QVariant coordvar)
{
    qDebug() << coordvar<< "sendUAVToLocn";
    uavListCpp[0]->setNextLocn(coordvar.value<QGeoCoordinate>());
    uavListCpp[0]->setDestnLocn(coordvar.value<QGeoCoordinate>());


}



void MainWindow::on_actionGeoFence_Breach_triggered()
{

    UAVList.clear();
    uavListCpp.clear();
    setCurrentTestCase("GeoFence Breach");
    flightmapitem->setProperty("testCase",currentTestCase());
    UAVNs::UAV *geoFenceUav = new UAVNs::UAV();;
    QGeoCoordinate homeLocn = QGeoCoordinate( 42.384661,-83.040787,0);

    geoFenceUav->setHomeLocn(QVariant::fromValue(homeLocn));
    geoFenceUav->setCurrentLocn(homeLocn);
    geoFenceUav->setID(1);
    geoFenceUav->setIconAddress("qrc:/qml_maps/icons/Insight yellow icon.png");
    geoFenceUav->setUavType(UAVNs::UAV::multiCopter);
    QVariantList flightPath;
    flightPath.push_back(geoFenceUav->homeLocn());
    QGeoCoordinate coord = homeLocn;
    coord.setAltitude(100);
    flightPath.push_back(QVariant::fromValue(coord));
    //p1
    coord =  QGeoCoordinate(42.384206,-83.040977,100);
    flightPath.push_back(QVariant::fromValue(coord));
    //p2
    coord =  QGeoCoordinate( 42.384648,-83.042726,100);
    flightPath.push_back(QVariant::fromValue(coord));
    //p3
    coord =  QGeoCoordinate(  42.385225,-83.046163,100);
    flightPath.push_back(QVariant::fromValue(coord));
    //p4
    coord =  QGeoCoordinate(   42.383961,-83.050965,100);
    flightPath.push_back(QVariant::fromValue(coord));

    //p5
    coord =  QGeoCoordinate(    42.382192,-83.049815,100);
    flightPath.push_back(QVariant::fromValue(coord));

    //p6
    coord =  QGeoCoordinate(    42.382423,-83.045533,100);
    flightPath.push_back(QVariant::fromValue(coord));

    //p7
    coord =  QGeoCoordinate(    42.382887,-83.041158,100);
    flightPath.push_back(QVariant::fromValue(coord));


    geoFenceUav->setFlightPath(flightPath);
    geoFenceUav->setIndexReached(0);
    geoFenceUav->setNextLocn(flightPath[1].value<QGeoCoordinate>());
    geoFenceUav->setDestnLocn(flightPath.last().value<QGeoCoordinate>());
    geoFenceUav->setHorSpeed(5);
    geoFenceUav->setVertSpeed(5);
    UAVList.append(geoFenceUav);
    uavListCpp.push_back(geoFenceUav);
    flightmapitem->setProperty("uavList",QVariant::fromValue(UAVList));
    QMetaObject::invokeMethod(flightmapitem,"dispUAVsFromList");
    startSimulation();
}

void MainWindow::on_actionDrone_Registration_triggered()
{

    if(regn == Q_NULLPTR)
        regn = new DroneRegistrationForm();
    connect(regn,SIGNAL(droneFormSubmitted()),this,SLOT(onDroneRegnFormSubmitted()));
    regn->move(desktop->screen()->rect().center() - regn->rect().center());
    regn->show();


}

void MainWindow::onDroneRegnFormSubmitted()
{

    return;
}

void MainWindow::on_actionNo_Fly_Zone_triggered()
{

    QFile flightPlanFile;
    QString path = QDir::homePath() + "/AirTrafficController/nofly.txt";
    flightPlanFile.setFileName(path);

    if(!flightPlanFile.open(QIODevice::ReadOnly)){
        qDebug() << "unable to read file";
    }
    QTextStream read;
    QVariantList noFlyFlightPlan,noFlyZonePolyCoords;
    read.setDevice(&flightPlanFile);
    QString line;
    while(!read.atEnd()){
        line = read.readLine();
        QStringList coordinateList = line.split("\t");
        if(coordinateList.length() == 4){
            QGeoCoordinate coord;
            coord.setLatitude(coordinateList[1].toDouble());
            coord.setLongitude(coordinateList[2].toDouble());
            noFlyFlightPlan.push_back(qVariantFromValue(coord));

        }
        else if(coordinateList.length() == 2){
            QGeoCoordinate coord;
            coord.setLatitude(coordinateList[0].toDouble());
            coord.setLongitude(coordinateList[1].toDouble());
            noFlyZonePolyCoords.push_back(qVariantFromValue(coord));

        }
    }
    flightPlanFile.close();
    QVariantList path2,path3;
    path3.push_back(qVariantFromValue(QGeoCoordinate(42.385590,-83.047605)));
    path3.push_back(qVariantFromValue(QGeoCoordinate(42.379502,-83.052600)));
    path2 = readFlightPlanfromfile(QDir::homePath() + "/AirTrafficController/fplan1.txt");
    QMetaObject::invokeMethod(flightmapitem,"dispNoFlyZoneTestCase",Q_ARG(QVariant,noFlyFlightPlan),Q_ARG(QVariant,noFlyZonePolyCoords),Q_ARG(QVariant,path2),Q_ARG(QVariant,path3));

}


QVariantList MainWindow::readFlightPlanfromfile(QString filename)
{

    QFile file;
    file.setFileName(filename);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "unable to read" << file.errorString();
    }
    QTextStream read;
    read.setDevice(&file);
    QVariantList path;
    QString line;
    while(!read.atEnd()){
        line = read.readLine();
        QStringList coordinateList = line.split("\t");
        if(coordinateList.length() == 4){
            QGeoCoordinate coord;
            coord.setLatitude(coordinateList[1].toDouble());
            coord.setLongitude(coordinateList[2].toDouble());
            path.push_back(qVariantFromValue(coord));

        }
    }
    file.close();
    return path;

}
