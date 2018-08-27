import QtQuick          2.6
import QtQuick.Controls 1.4
import QtLocation       5.6
import QtPositioning    5.5
import "../qml_maps"


Rectangle{
    id : _vehicleData
    property string uavid
    width: _id.width*1.2
    height: _id.height*1.2
    color: "black"
    opacity: 0.7
    Text{
        id: _id
        text: uavid
        color: "white"
        font.pointSize: 12
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
