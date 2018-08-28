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

    if(uavListCpp[0]->currentLocn().distanceTo(uavListCpp[0]->nextLocn()) > 10){
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

    QMetaObject::invokeMethod(flightmapitem,"clearallitems");
    UAVList.clear();
    uavListCpp.clear();
    setCurrentTestCase("GeoFence Breach");
    flightmapitem->setProperty("testCase",currentTestCase());
    UAVNs::UAV *geoFenceUav = new UAVNs::UAV();;
    QGeoCoordinate homeLocn = QGeoCoordinate( 42.384661,-83.040787,0);

    geoFenceUav->setHomeLocn(QVariant::fromValue(homeLocn));
    geoFenceUav->setCurrentLocn(homeLocn);
    geoFenceUav->setID(1);
    geoFenceUav->setIconAddress("qrc:/qml_maps/icons/D2.png");
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
    QMetaObject::invokeMethod(flightmapitem,"clearallitems");

    setCurrentTestCase("NO-FlyZone");
    flightmapitem->setProperty("NO-FlyZone",currentTestCase());

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



void MainWindow::on_actionMid_Air_Collision_Avoidance_triggered()
{
    QMetaObject::invokeMethod(flightmapitem,"clearallitems");

    UAVList.clear();
    uavListCpp.clear();
    collisionPossible = false;
    setCurrentTestCase("Mid-Air Collision");
    flightmapitem->setProperty("Mid-Air Collision",currentTestCase());

    qDebug() << "mid air collision";
    //    UAVNs::UAV *uav4 = new UAVNs::UAV();

    QVariantList flpath;
    flpath.push_back(qVariantFromValue(QGeoCoordinate(42.386151,-83.039228)));
    flpath.push_back(qVariantFromValue(QGeoCoordinate(42.3851446,-83.0413628)));
    UAVNs::UAV *uav1 = createUAV(1,QGeoCoordinate(42.386151,-83.039228),"qrc:/qml_maps/icons/airambulance.png",flpath);

    QVariantList flpath2,flpath3;
    flpath2 = readFlightPlanfromfile(QDir::homePath() + "/AirTrafficController/fplan1.txt");
    UAVNs::UAV *uav2 = createUAV(2,flpath2[0].value<QGeoCoordinate>(),"qrc:/qml_maps/icons/D2.png",flpath2);

    flpath3.push_back( qVariantFromValue(QGeoCoordinate(42.382850,-83.041913)));
    flpath3.push_back(qVariantFromValue(QGeoCoordinate(42.385932,-83.043022)) );

    UAVNs::UAV *uav3 = createUAV(3,flpath3[0].value<QGeoCoordinate>(),"qrc:/qml_maps/icons/imageedit_1_3684434905.png",flpath3);

    UAVList.append(uav1);
    UAVList.append(uav2);
    UAVList.append(uav3);
    uavListCpp.push_back(uav1);
    uavListCpp.push_back(uav2);
    uavListCpp.push_back(uav3);

    flightmapitem->setProperty("uavList",QVariant::fromValue(UAVList));
    QMetaObject::invokeMethod(flightmapitem,"dispUAVsFromList");

    startSimulationTraffic();

}


UAVNs::UAV *MainWindow::createUAV(int id, QGeoCoordinate home, QString iconAddr, QVariantList flpath)
{
    UAVNs::UAV *uav1 = new UAVNs::UAV();
    uav1->setID(id);
    uav1->setHomeLocn(qVariantFromValue(home));
    uav1->setIconAddress(iconAddr);
    uav1->setUavType(UAVNs::UAV::multiCopter);

    uav1->setFlightPath(flpath);
    uav1->setCurrentLocn(home);
    uav1->setNextLocn(flpath[1].value<QGeoCoordinate>());
    uav1->setDestnLocn(flpath.last().value<QGeoCoordinate>());
    uav1->setHorSpeed(5);
    uav1->setIndexReached(0);
    return uav1;
}



void MainWindow::startSimulationTraffic()
{

    qDebug() << "startSimulationTraffic";
    if(midAirTimer1 == Q_NULLPTR){
        midAirTimer1 = new QTimer(this);
    }
    if(midAirTimer2 == Q_NULLPTR){
        midAirTimer2 = new QTimer(this);
    }
    if(midAirTimer3 == Q_NULLPTR){
        midAirTimer3 = new QTimer(this);
    }
    connect(midAirTimer1,SIGNAL(timeout()),this,SLOT(goToNextLocnMidAir1()));
    midAirTimer1->start(1000);

    connect(midAirTimer2,SIGNAL(timeout()),this,SLOT(goToNextLocnMidAir2()));
    midAirTimer2->start(1000);
    connect(midAirTimer3,SIGNAL(timeout()),this,SLOT(goToNextLocnMidAir3()));
    midAirTimer3->start(1000);
}

void MainWindow::goToNextLocnMidAir1()
{

    //    for(int i=0;i<1;i++){
    int i=0;
    if(uavListCpp[i]->currentLocn().distanceTo(uavListCpp[i]->nextLocn()) > 10){
        QGeoCoordinate nLocn;
        double dist = 10;
        double azim = uavListCpp[i]->currentLocn().azimuthTo(uavListCpp[i]->nextLocn());
        nLocn = uavListCpp[i]->currentLocn().atDistanceAndAzimuth(dist,azim);
        uavListCpp[0]->setCurrentLocn(nLocn);
        QMetaObject::invokeMethod(flightmapitem,"vehDataChangedList",Q_ARG(QVariant,qVariantFromValue(nLocn)),Q_ARG(QVariant,qVariantFromValue(i)));
        checkIfDroneLiesWithinDia(30);
        if(nLocn.distanceTo(uavListCpp[i]->destnLocn()) < 10){
            midAirTimer1->stop();

        }
    }
    else{
        qDebug() << "reached" << uavListCpp[0]->indexReached();

        if(uavListCpp[i]->indexReached() +1 < uavListCpp[i]->flightPath().length()){

            uavListCpp[i]->setNextLocn(uavListCpp[i]->flightPath()[uavListCpp[i]->indexReached() +1].value<QGeoCoordinate>());
            uavListCpp[i]->setIndexReached(uavListCpp[i]->indexReached() +1);
        }
        else{
            midAirTimer1->stop();
            qDebug() << "reached destn";
        }
    }

    //    }
}

void MainWindow::goToNextLocnMidAir2()
{
    //    qDebug() << "goToNextLocnMidAir2";
    //    for(int i=0;i<1;i++){
    int i=1;
    if(uavListCpp[i]->currentLocn().distanceTo(uavListCpp[i]->nextLocn()) > 10){
        QGeoCoordinate nLocn;
        double dist = 10;
        double azim = uavListCpp[i]->currentLocn().azimuthTo(uavListCpp[i]->nextLocn());
        nLocn = uavListCpp[i]->currentLocn().atDistanceAndAzimuth(dist,azim);
        uavListCpp[i]->setCurrentLocn(nLocn);
        QMetaObject::invokeMethod(flightmapitem,"vehDataChangedList",Q_ARG(QVariant,qVariantFromValue(nLocn)),Q_ARG(QVariant,qVariantFromValue(i)));
        checkIfDroneLiesWithinDia(30);
        if(nLocn.distanceTo(uavListCpp[i]->destnLocn()) < 10){
            midAirTimer2->stop();
            qDebug() << "reached destn";

        }
    }
    else{
        qDebug() << "reached" << uavListCpp[i]->indexReached();

        if(uavListCpp[i]->indexReached() +1 < uavListCpp[i]->flightPath().length()){

            uavListCpp[i]->setNextLocn(uavListCpp[i]->flightPath()[uavListCpp[i]->indexReached() +1].value<QGeoCoordinate>());
            uavListCpp[i]->setIndexReached(uavListCpp[i]->indexReached() +1);
        }
        else{
            midAirTimer2->stop();
            qDebug() << "reached destn";
        }

    }



}

void MainWindow::goToNextLocnMidAir3()
{
    //    for(int i=0;i<1;i++){
    int i=2;
    if(uavListCpp[i]->currentLocn().distanceTo(uavListCpp[i]->nextLocn()) > 10){
        QGeoCoordinate nLocn;
        double dist = 10;
        double azim = uavListCpp[i]->currentLocn().azimuthTo(uavListCpp[i]->nextLocn());
        nLocn = uavListCpp[i]->currentLocn().atDistanceAndAzimuth(dist,azim);
        uavListCpp[i]->setCurrentLocn(nLocn);
        QMetaObject::invokeMethod(flightmapitem,"vehDataChangedList",Q_ARG(QVariant,qVariantFromValue(nLocn)),Q_ARG(QVariant,qVariantFromValue(i)));
        checkIfDroneLiesWithinDia(30);
        if(nLocn.distanceTo(uavListCpp[i]->destnLocn()) < 10){
            midAirTimer3->stop();
            qDebug() << "reached destn";

        }
    }
    else{
        qDebug() << "reached" << uavListCpp[i]->indexReached();

        if(uavListCpp[i]->indexReached() +1 < uavListCpp[i]->flightPath().length()){

            uavListCpp[i]->setNextLocn(uavListCpp[i]->flightPath()[uavListCpp[i]->indexReached() +1].value<QGeoCoordinate>());
            uavListCpp[i]->setIndexReached(uavListCpp[i]->indexReached() +1);
        }
        else{
            midAirTimer3->stop();
            qDebug() << "reached destn";
        }

    }
}



void MainWindow::checkIfDroneLiesWithinDia(float dia)
{
    //0 1

    if(!collisionPossible){
        dia =80;
        if(uavListCpp[0]->currentLocn().distanceTo(uavListCpp[1]->currentLocn()) < dia){

            qDebug() << "01";
            handleAvoidance(0,1);
        }
        //0 2
        if(uavListCpp[0]->currentLocn().distanceTo(uavListCpp[2]->currentLocn()) < dia){
            qDebug() << "02";
            handleAvoidance(0,2);
        }
        // 1 2
        if(uavListCpp[1]->currentLocn().distanceTo(uavListCpp[2]->currentLocn()) < dia){
            qDebug() << "12";
            handleAvoidance(1,2);
        }
    }

}

void MainWindow::handleAvoidance(int i, int j)
{
    qDebug() << "handleAvoidance" << i << j;
    midAirTimer2->stop();
    collisionPossible = true;
    if(collTimer == Q_NULLPTR){
        collTimer = new QTimer(this);
        collTimer->setInterval(5000);
        collTimer->setSingleShot(true);
        connect(collTimer,SIGNAL(timeout()),this,SLOT(restartDrone()));
    }
    collTimer->start();
    QMetaObject::invokeMethod(flightmapitem,"dispAvoidanceRect");
}

void MainWindow::restartDrone()
{
    qDebug() << "restartDrone";
    midAirTimer2->start();
    collisionPossible = true;
}
