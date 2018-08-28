#include "droneregnprocess.h"
#include "ui_droneregnprocess.h"

DroneRegnProcess::DroneRegnProcess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DroneRegnProcess)
{
    ui->setupUi(this);
}

DroneRegnProcess::~DroneRegnProcess()
{
    delete ui;
}
