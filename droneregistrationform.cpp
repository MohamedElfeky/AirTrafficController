#include "droneregistrationform.h"
#include "ui_droneregistrationform.h"

DroneRegistrationForm::DroneRegistrationForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DroneRegistrationForm)
{
    ui->setupUi(this);
    setWindowTitle("Drone Registration Form");
    QStringList list;
    list << "MultiCopter" << "Hybrid" << "FixedWing";
    ui->dronemftext->setText("Company X");
    ui->digiSign->setText("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    ui->uavtypelist->addItems(list);
    ui->uavtypelist->setCurrentIndex(0);
}

DroneRegistrationForm::~DroneRegistrationForm()
{
    delete ui;
}

void DroneRegistrationForm::on_buttonBox_accepted()
{
    droneFormSubmitted();

}
