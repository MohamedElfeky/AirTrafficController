import QtQuick          2.6
import QtQuick.Controls 1.4
import QtLocation       5.6
import QtPositioning    5.5
import "../qml_maps"

Rectangle{
    id :_dispRect
    width: 400
    height: 50
    border.color: "black"
    visible:  false
    property string imageName: ""
    property string coord: ""
    Text{
        id : coordinateText
        text:  coord
        font.bold: true
        font.pointSize: 12
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }
    //    Column{
    //        Text{
    //            id : coordinateText
    //            text:  coord
    //            font.bold: true
    //            font.pointSize: 12
    //            verticalAlignment: Text.AlignVCenter
    //            horizontalAlignment: Text.AlignHCenter
    //            anchors.horizontalCenter: parent.horizontalCenter

    //        }
    //        Text{
    //            id: imageNameText
    //            text: imageName
    //            verticalAlignment: Text.AlignVCenter
    //            horizontalAlignment: Text.AlignHCenter
    //            font.pointSize: 12
    //            anchors.horizontalCenter: parent.horizontalCenter
    //        }
    //    }

}
