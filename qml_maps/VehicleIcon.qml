import QtQuick 2.4
import QtLocation       5.3
import QtPositioning    5.3
import "../qml_maps"
MapQuickItem {

    id: _markeritem
    property real ang: 0.0
    property real imageWidth: 48
    property real imageHeight: 48
    property string imagePath
    anchorPoint.x:  vehicleIcon.width  / 2
    anchorPoint.y:  vehicleIcon.height / 2
    sourceItem:Rectangle{
        id : vehicleIcon
        width: 1.1*imageWidth
        height: 1.1*imageHeight
        radius: 0.5*width
        color: "transparent"
        Image {
            id: vehicleIconImage
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            source: imagePath
            mipmap:     true
            sourceSize.width: imageWidth
            sourceSize.height: imageHeight

            fillMode:   Image.PreserveAspectFit
            transform: Rotation {
                origin.x:   vehicleIconImage.width  / 2
                origin.y:   vehicleIconImage.height / 2

                angle:      ang
            }
        }
    }
}
