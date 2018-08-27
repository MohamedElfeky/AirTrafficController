#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Air Traffic Controller");
    showMap();
    setUpDroneEnv(3);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMap(){

    mapView = new QQuickView;
    mapView->setSource(QUrl(QStringLiteral("qrc:/qml_maps/FlightMapViewer.qml")));

    mapView->setResizeMode(QQuickView::SizeRootObjectToView);

    if (mapView->status() == QQuickView::Error) {
        qWarning() << mapView->errors();
        return;
    }
    mapwidget = QWidget::createWindowContainer(mapView, this);
    ui->maingridLayout->addWidget(mapwidget);
    setFocusProxy(mapwidget); // focus container widget when top level widget is focused
    //resize(size);
    setFocusPolicy(Qt::NoFocus);

    mapView->rootObject()->setProperty("width",this->width());
    mapView->rootObject()->setProperty("height",this->height());
    flightmapitem = mapView->rootObject()->findChild<QQuickItem*>("map_main");
    mapView->rootContext()->setContextProperty("_mainWindow",this);

    //    addMapTypes();
    //    connect(this,SIGNAL(MapTypeChanged(QVariant)),flightmapitem,SLOT(changeMapType(QVariant)));

    QMainWindow::showMaximized();
    QApplication::processEvents();

}

void MainWindow::setUpDroneEnv(int numDrones)
{

    qDebug() << "numDrones" << numDrones;
    UAVList.clear();
    for(int i=0; i< numDrones;i++){

        QGeoCoordinate homeLocn = getRandomHomeLocn(1300,800,numDrones+10,1000+i);

        if(!homeLocn.isValid()){
            qDebug() << homeLocn << homeLocn.isValid()<< "homelocns";
            homeLocn = getRandomHomeLocn(1300,800,numDrones+5,(3000+i));
        }
        //        UAVNs::UAV uav =  uav(i,"qrc:/qml_maps/icons/Insight yellow icon.png", UAVNs::UAV::multiCopter,homeLocn);
        UAVNs::UAV *uav = new UAVNs::UAV();
        uav->setID(i);
        uav->setIconAddress("qrc:/qml_maps/icons/Insight yellow icon.png");
        uav->setUavType(UAVNs::UAV::multiCopter);
        uav->setHomeLocn(QVariant::fromValue(homeLocn));
        QVariantList flPath = generateRandomFlightPath(homeLocn,3000+i);
        uav->setFlightPath(flPath);
        uav->setCurrentLocn(homeLocn);
        uav->setNextLocn(flPath[1].value<QGeoCoordinate>());
        uav->setDestnLocn(flPath.last().value<QGeoCoordinate>());

        uav->setHorSpeed(5);
        uav->setIndexReached(0);
        UAVList.append(uav);
        uavListCpp.push_back(uav);
        //        qDebug() << uav.homeLocn() <<"uav.homeLocn";
        //        QVariant uavV = QVariant::fromValue(uav);
        //        qDebug() << uavV.value<UAVNs::UAV>().ID() << "ID";

    }

    flightmapitem->setProperty("uavList",QVariant::fromValue(UAVList));
    QMetaObject::invokeMethod(flightmapitem,"dispUAVsFromList");

//    startSimulation();
}

QGeoCoordinate MainWindow::getRandomHomeLocn(qreal width, qreal height, qreal g, int seed)
{
    //a = width, b = height
    qreal wm,hm;
    if(width > height){
        hm = g;
        wm = qFloor(g*width/height);
    }
    else{
        wm = g;
        hm = qFloor(g*height/width);
    }

    float wrand,hrand;
    srand(seed);
    wrand = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/wm));
    hrand = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/hm));
    qreal wv = width * wrand/wm;
    qreal hv = height* hrand/hm;
    QVariant coordVar;

    QMetaObject::invokeMethod(flightmapitem,"getCoordinateFromPt",Q_RETURN_ARG(QVariant, coordVar),Q_ARG(QVariant, QVariant::fromValue(wv)), Q_ARG(QVariant, QVariant::fromValue(hv)));

    return coordVar.value<QGeoCoordinate>();
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

    //    qDebug() << uavListCpp[0]->currentLocn().distanceTo(uavListCpp[0]->nextLocn()) <<  "goToNextLocn";
    if(uavListCpp[0]->currentLocn().distanceTo(uavListCpp[0]->nextLocn()) > 5){
        QGeoCoordinate nLocn;
        double dist = 5;
        double azim = uavListCpp[0]->currentLocn().azimuthTo(uavListCpp[0]->nextLocn());
        nLocn = uavListCpp[0]->currentLocn().atDistanceAndAzimuth(dist,azim);
        uavListCpp[0]->setCurrentLocn(nLocn);
        QMetaObject::invokeMethod(flightmapitem,"vehDataChanged",Q_ARG(QVariant,qVariantFromValue(nLocn)));
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
}


