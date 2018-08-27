import QtQuick          2.7
import QtLocation       5.6
import QtPositioning    5.5
import QtQml 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2

import QtQuick.Controls 2.0

import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import "../qml_maps"

Rectangle{


    id: rect
    objectName: "rect_viewer"
    width: 1300
    height: 800

    property bool enableSpinbox: false
    FlightMap{
        id: _flightMap
        objectName: "map_main"
    }




    SpinBox{
        id: mapItemSize
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 100
        z : 3
        value: 10
        from: 2
        to : 30
        stepSize: 1
        editable: true
        validator: IntValidator {
            locale: mapItemSize.locale.name
            bottom: Math.min(mapItemSize.from, mapItemSize.to)
            top: Math.max(mapItemSize.from, mapItemSize.to)
        }

        contentItem: TextInput {
            z: 2
            text: mapItemSize.textFromValue(mapItemSize.value, mapItemSize.locale)

            font: mapItemSize.font
            color: "#21be2b"
            selectionColor: "#21be2b"
            selectedTextColor: "#ffffff"
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter

            readOnly: !mapItemSize.editable
            validator: mapItemSize.validator
            inputMethodHints: Qt.ImhFormattedNumbersOnly
        }

        up.indicator: Rectangle {
            x: mapItemSize.mirrored ? 0 : parent.width - width
            height: parent.height
            implicitWidth: 15
            implicitHeight: 15
            color: mapItemSize.up.pressed ? "#e4e4e4" : "#f6f6f6"
            border.color: enabled ? "#21be2b" : "#bdbebf"

            Text {
                text: "+"
                font.pixelSize: mapItemSize.font.pixelSize * 2
                color: "#21be2b"
                anchors.fill: parent
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        down.indicator: Rectangle {
            x: mapItemSize.mirrored ? parent.width - width : 0
            height: parent.height
            implicitWidth: 15
            implicitHeight: 15
            color: mapItemSize.down.pressed ? "#e4e4e4" : "#f6f6f6"
            border.color: enabled ? "#21be2b" : "#bdbebf"

            Text {
                text: "-"
                font.pixelSize: mapItemSize.font.pixelSize * 2
                color: "#21be2b"
                anchors.fill: parent
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        background: Rectangle {
            implicitWidth: 100
            border.color: "#bdbebf"
        }


        visible: enableSpinbox
        onValueChanged:{
            _flightMap.changeMarkerSize(mapItemSize.value)
        }

    }//spinbox minzoom ends

}
