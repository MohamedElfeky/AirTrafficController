#ifndef DRONEREGISTRATIONFORM_H
#define DRONEREGISTRATIONFORM_H

#include <QDialog>

namespace Ui {
class DroneRegistrationForm;
}

class DroneRegistrationForm : public QDialog
{
    Q_OBJECT

public:
    explicit DroneRegistrationForm(QWidget *parent = 0);
    ~DroneRegistrationForm();

signals:
    void droneFormSubmitted();
private slots:
    void on_buttonBox_accepted();

private:
    Ui::DroneRegistrationForm *ui;
};

#endif // DRONEREGISTRATIONFORM_H
