import QtQuick          2.7
import QtQuick.Controls 1.4
import QtLocation       5.6
import QtPositioning    5.5
import QtQml 2.2
import QtQuick.Dialogs 1.2

import "../qml_maps"

MapPolyline{
    id:_contourLine
    property string col: "red"
    property var pathList: []
    //    property string fileName: ""
    line.width: 2.5
    line.color: col
    path: pathList
    //    MouseArea{
    //        enabled: true
    //        onEntered: {
    //            console.log(fileName)
    //        }
    //    }
}
