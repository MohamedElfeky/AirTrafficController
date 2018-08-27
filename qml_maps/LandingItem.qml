import QtQuick          2.6
import QtQuick.Controls 1.4
import QtLocation       5.6
import QtPositioning    5.5
import "../qml_maps"


MapQuickItem{
    id:_landingItem
    property string uavID
    anchorPoint.x: _markerImage.width/2
    anchorPoint.y: _markerImage.height/2
    width: sourceItem.width
    height: sourceItem.height
    zoomLevel: 17
    sourceItem: Rectangle{
        id: _markerImage
        width: 30
        height: 30
        radius: /*2.75*/width*0.5
        //                radius: width*0.5
        color: "yellow"
        border.color: "transparent"
        border.width: 3
        opacity: 1.0
        Text{
            text: uavID
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: "black"
            font.bold: true
            font.pointSize: 16
        }

        //        Drag.active:_markerMouseArea.drag.active

    }
}
