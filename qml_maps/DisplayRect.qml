import QtQuick          2.6
import QtQuick.Controls 1.4
import QtLocation       5.6
import QtPositioning    5.5
import "../qml_maps"

Rectangle{
    id :_dispRect
    width: coordinateText.width + 10
    height: 30
    border.color: "black"
    visible:  false
    property string imageName: ""
    property string coord: ""
    Column{
        Text{
            id : coordinateText
            text:  coord
            anchors.horizontalCenter: parent.horizontalCenter

        }
        Text{
            id: imageNameText
            text: imageName
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

}
