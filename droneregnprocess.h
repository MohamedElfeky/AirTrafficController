#ifndef DRONEREGNPROCESS_H
#define DRONEREGNPROCESS_H

#include <QWidget>

namespace Ui {
class DroneRegnProcess;
}

class DroneRegnProcess : public QWidget
{
    Q_OBJECT

public:
    explicit DroneRegnProcess(QWidget *parent = 0);
    ~DroneRegnProcess();

private:
    Ui::DroneRegnProcess *ui;
};

#endif // DRONEREGNPROCESS_H
