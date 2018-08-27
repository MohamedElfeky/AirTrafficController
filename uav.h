#ifndef UAV_H
#define UAV_H

#include <QObject>
#include <QGeoCoordinate>
#include<QVariant>
#include<QVariantList>
#include<QList>
#include <QMetaType>

namespace UAVNs {
class UAV : public QObject
{
    Q_OBJECT


public:

    enum priority{
        High,
        Medium,
        Low
    };

    enum uavModelType{
        multiCopter,
        fixedWing,
        hybrid
    };
    Q_ENUM(priority)
    Q_ENUM(uavModelType)
    UAV(QObject *parent = nullptr);
    UAV(int id, QString iconAdd, uavModelType uavtype, QVariant homelocn, QObject *parent= nullptr);
    UAV(const UAV &other);
    UAV operator = (const UAV &other);
    ~UAV();
    Q_PROPERTY(QGeoCoordinate currentLocn READ currentLocn WRITE setCurrentLocn NOTIFY currentLocnChanged)
    Q_PROPERTY(QGeoCoordinate nextLocn READ nextLocn WRITE setNextLocn NOTIFY nextLocnChanged)
    Q_PROPERTY(QVariantList flightPath READ flightPath WRITE setFlightPath NOTIFY flightPathChanged)
    Q_PROPERTY(QVariant homeLocn READ homeLocn WRITE setHomeLocn NOTIFY homeLocnChanged)
    Q_PROPERTY(QGeoCoordinate destnLocn READ destnLocn WRITE setDestnLocn NOTIFY destnLocnChanged)
    Q_PROPERTY(QString mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(bool armStatus READ armStatus WRITE setArmStatus NOTIFY armStatusChanged)
    Q_PROPERTY(qreal horSpeed READ horSpeed WRITE setHorSpeed NOTIFY horSpeedChanged)
    Q_PROPERTY(qreal vertSpeed READ vertSpeed WRITE setVertSpeed NOTIFY vertSpeedChanged)
    Q_PROPERTY(QString iconAddress READ iconAddress WRITE setIconAddress NOTIFY iconAddressChanged)
    Q_PROPERTY(int ID READ ID WRITE setID NOTIFY IDChanged)
    Q_PROPERTY(uavModelType uavType READ uavType WRITE setUavType NOTIFY uavTypeChanged)
    Q_PROPERTY(priority priorityType READ priorityType WRITE setPriorityType NOTIFY priorityTypeChanged)
    Q_PROPERTY(int indexReached READ indexReached WRITE setIndexReached NOTIFY indexReachedChanged)


    QGeoCoordinate currentLocn() const
    {
        return m_currentLocn;
    }

    QGeoCoordinate nextLocn() const
    {
        return m_nextLocn;
    }

    QVariantList flightPath() const
    {
        return m_flightPath;
    }

    QVariant homeLocn() const
    {
        return m_homeLocn;
    }

    QGeoCoordinate destnLocn() const
    {
        return m_destnLocn;
    }

    QString mode() const
    {
        return m_mode;
    }

    bool armStatus() const
    {
        return m_armStatus;
    }

    qreal horSpeed() const
    {
        return m_horSpeed;
    }

    qreal vertSpeed() const
    {
        return m_vertSpeed;
    }

    QString iconAddress() const
    {
        return m_iconAddress;
    }

    int ID() const
    {
        return m_ID;
    }

    uavModelType uavType() const
    {
        return m_uavType;
    }

    priority priorityType() const
    {
        return m_priorityType;
    }

    int indexReached() const
    {
        return m_indexReached;
    }

signals:

    void currentLocnChanged(QGeoCoordinate currentLocn);

    void nextLocnChanged(QGeoCoordinate nextLocn);

    void flightPathChanged(QVariantList flightPath);

    void homeLocnChanged(QVariant homeLocn);

    void destnLocnChanged(QGeoCoordinate destnLocn);

    void modeChanged(QString mode);

    void armStatusChanged(bool armStatus);

    void horSpeedChanged(qreal horSpeed);

    void vertSpeedChanged(qreal vertSpeed);

    void iconAddressChanged(QString iconAddress);

    void IDChanged(int ID);

    void uavTypeChanged(uavModelType uavType);

    void priorityTypeChanged(priority priorityType);

    void indexReachedChanged(int indexReached);

public slots:
    void setCurrentLocn(QGeoCoordinate currentLocn)
    {
        if (m_currentLocn == currentLocn)
            return;

        m_currentLocn = currentLocn;
        emit currentLocnChanged(m_currentLocn);
    }
    void setNextLocn(QGeoCoordinate nextLocn)
    {
        if (m_nextLocn == nextLocn)
            return;

        m_nextLocn = nextLocn;
        emit nextLocnChanged(m_nextLocn);
    }
    void setFlightPath(QVariantList flightPath)
    {
        if (m_flightPath == flightPath)
            return;

        m_flightPath = flightPath;
        emit flightPathChanged(m_flightPath);
    }
    void setHomeLocn(QVariant homeLocn)
    {
        if (m_homeLocn == homeLocn)
            return;

        m_homeLocn = homeLocn;
        emit homeLocnChanged(m_homeLocn);
    }
    void setDestnLocn(QGeoCoordinate destnLocn)
    {
        if (m_destnLocn == destnLocn)
            return;

        m_destnLocn = destnLocn;
        emit destnLocnChanged(m_destnLocn);
    }
    void setMode(QString mode)
    {
        if (m_mode == mode)
            return;

        m_mode = mode;
        emit modeChanged(m_mode);
    }
    void setArmStatus(bool armStatus)
    {
        if (m_armStatus == armStatus)
            return;

        m_armStatus = armStatus;
        emit armStatusChanged(m_armStatus);
    }
    void setHorSpeed(qreal horSpeed)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_horSpeed, horSpeed))
            return;

        m_horSpeed = horSpeed;
        emit horSpeedChanged(m_horSpeed);
    }
    void setVertSpeed(qreal vertSpeed)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_vertSpeed, vertSpeed))
            return;

        m_vertSpeed = vertSpeed;
        emit vertSpeedChanged(m_vertSpeed);
    }
    void setIconAddress(QString iconAddress)
    {
        if (m_iconAddress == iconAddress)
            return;

        m_iconAddress = iconAddress;
        emit iconAddressChanged(m_iconAddress);
    }
    void setID(int ID)
    {
        if (m_ID == ID)
            return;

        m_ID = ID;
        emit IDChanged(m_ID);
    }
    void setUavType(uavModelType uavType)
    {
        if (m_uavType == uavType)
            return;

        m_uavType = uavType;
        emit uavTypeChanged(m_uavType);
    }
    void setPriorityType(priority priorityType)
    {
        if (m_priorityType == priorityType)
            return;

        m_priorityType = priorityType;
        emit priorityTypeChanged(m_priorityType);
    }

    void setIndexReached(int indexReached)
    {
        if (m_indexReached == indexReached)
            return;

        m_indexReached = indexReached;
        emit indexReachedChanged(m_indexReached);
    }

private:

    QGeoCoordinate m_currentLocn;

    QGeoCoordinate m_nextLocn;

    QVariantList m_flightPath;

    QVariant m_homeLocn;

    QGeoCoordinate m_destnLocn;

    QString m_mode = "STABILIZE";

    bool m_armStatus = false;

    qreal m_horSpeed =0;

    qreal m_vertSpeed=0;

    QString m_iconAddress;

    int m_ID;

    uavModelType m_uavType;

    priority m_priorityType;

    int m_indexReached;
};
}

Q_DECLARE_METATYPE(UAVNs::UAV)

#endif // UAV_H
