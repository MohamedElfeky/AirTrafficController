import QtQuick          2.6
import QtQuick.Controls 1.4
import QtLocation       5.6
import QtPositioning    5.5
import "../qml_maps"


MapQuickItem{
    id: _marker
    anchorPoint.x: _markerImage.width/2
    anchorPoint.y: _markerImage.height/2
    width: sourceItem.width
    height: sourceItem.height
    zoomLevel: 17

    z: 4

    //    signal          entered()
    //    signal          exited()

    property string fileNametoDisp : ""

    sourceItem: Rectangle{
        id: _markerImage
        width: 600
        height: 100
        visible: true
        Text {
            id: _fileDispName
            text: qsTr(fileNametoDisp)
            font.family: "Cambria"
            font.bold: true
            font.pointSize: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

        }


    }




}
