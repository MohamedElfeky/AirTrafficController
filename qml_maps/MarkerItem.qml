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
    Drag.active:_markerMouseArea.drag.active
    Drag.hotSpot.x: 10
    Drag.hotSpot.y: 10
    z: 4
    property int index: 0
    property real imageSize: 15
    //    signal          entered()
    //    signal          exited()

    property var dispRect

    sourceItem: Rectangle{
        id: _markerImage
        width: imageSize
        height: imageSize
        radius: /*2.75*/imageSize*0.25
        //                radius: width*0.5
        color: "yellow"
        border.color: "transparent"
        border.width: 3
        opacity: 1.0
        //        Drag.active:_markerMouseArea.drag.active

    }

    MouseArea{
        id: _markerMouseArea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            dispRect.visible = true
        }
        onExited: {
            dispRect.visible = false
        }
    }// mouse area ends


}
