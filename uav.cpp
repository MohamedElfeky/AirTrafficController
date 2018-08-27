#include "uav.h"

UAVNs::UAV::UAV(QObject *parent) : QObject(parent)
{

}

UAVNs::UAV::UAV(int id, QString iconAdd, UAV::uavModelType uavtype, QVariant homelocn, QObject *parent): QObject(parent)
{
    m_ID = id;
    m_iconAddress = iconAdd;
    m_uavType = uavtype;
    m_homeLocn = homelocn;
}

UAVNs::UAV::UAV(const UAV &other)
{
    m_ID = other.ID();
    m_iconAddress = other.iconAddress();
    m_uavType = other.uavType();
    m_homeLocn = other.homeLocn();
    m_currentLocn = other.currentLocn();
    m_nextLocn = other.nextLocn();
    m_flightPath = other.flightPath();
    m_destnLocn = other.destnLocn();
    m_mode = other.mode();
    m_armStatus = other.armStatus();
    m_horSpeed = other.horSpeed();
    m_vertSpeed = other.vertSpeed();
    m_priorityType = other.priorityType();



}





UAVNs::UAV::~UAV()
{

}

