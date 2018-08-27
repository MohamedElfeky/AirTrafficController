import QtQuick          2.7
import QtQuick.Controls 1.4
import QtLocation       5.6
import QtPositioning    5.5
import QtQml 2.2
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import UAV 1.0
import "../qml_maps"


Map{

    id:_map
    plugin: bingoplugin
    center : QtPositioning.coordinate(/*13.1554,	77.4868*/42.382658, -83.043852);
    zoomLevel: 17
    anchors.fill: parent
    gesture.acceptedGestures: MapGestureArea.PanGesture | MapGestureArea.FlickGesture | MapGestureArea.PinchGesture
    gesture.flickDeceleration: 3000
    gesture.enabled: true
    focus: true
    onCopyrightLinkActivated: Qt.openUrlExternally(link)
    signal geocodeFinished()
    Plugin {
        id: bingoplugin
        name: "bingomaps"
        locales: "en_EN"
    }

    property var coordsListtoDisplay: []
    property var coordsStringList: []
    property var imageNamesList: []
    property var coordsMarkerList: []
    property var dataDispList: []
    property var lineMarkersList: []
    property var colorList: []
    property var maptypeListName: []
    property string _activemapType: ""
    property var finalDataDispList: []
    property var finalCoordMarkerList: []
    property var filesList: []
    property var centerCoordsList: []
    property var fileNameDataDisplayList: []

    //atc
    property var uavList: []
    property var vehicleIconList: []
    property var idMarkersList: []
    property var pathMarkersList: []
    property var landMarkersList: []
    signal coordsAdded()


    function changeMapType(map_type){
        for(var i=0; i< _flightMap.supportedMapTypes.length;i++){
            if(_flightMap.supportedMapTypes[i].name === map_type)
                _flightMap.activeMapType = _flightMap.supportedMapTypes[i]
        }
    }



    function convertDMSToDD(degrees, minutes, seconds, direction) {
        var dd = degrees + minutes/60 + seconds/(60*60);

        if (direction === "S" || direction === "W") {
            dd = dd * -1;
        } // Don't do anything for N or E
        return dd;
    }

    function parseDMS(input) {
        var ip = input.toString()
        var parts = ip.split(",");
        var latDMS = parts[0].trim().split(" ")
        var lonDMS = parts[1].trim().split(" ")
        var alt = parts[2]
        var latDeg = latDMS[0].split("°")[0]
        var latmin = latDMS[1].split("'")[0]
        var latSec = latDMS[2].split('"')[0]
        var latDirn = latDMS[3]

        var lonDeg = lonDMS[0].split("°")[0]
        var lonmin = lonDMS[1].split("'")[0]
        var lonSec = lonDMS[2].split('"')[0]
        var lonDirn = lonDMS[3]
        //        console.log(latDirn,lonDirn)
        var lat = convertDMSToDD(parseFloat(latDeg),parseFloat(latmin),parseFloat(latSec),latDirn);
        var lng = convertDMSToDD(parseFloat(lonDeg),parseFloat(lonmin),parseFloat(lonSec),lonDirn);
        //        console.log(lat,lng,alt)
        return lat + "," + lng + "," + alt
    }

    function addCoordstomap(){

        if(coordsListtoDisplay.length == imageNamesList.length){

            for(var i=0;i< coordsListtoDisplay.length;i++){

                var comp = Qt.createComponent("DisplayRect.qml");
                if (comp.status === Component.Ready) {
                    var item = comp.createObject(_map);
                    item.imageName = imageNamesList[i]
                    item.coord = coordsStringList[i]
                    dataDispList.push(item)
                }


                var component = Qt.createComponent("MarkerItem.qml");
                if (component.status === Component.Ready) {
                    var markerItem = component.createObject(_map);
                    markerItem.coordinate =  coordsListtoDisplay[i]
                    markerItem.index = i
                    markerItem.imageSize = 10
                    markerItem.dispRect =dataDispList[i]
                    dataDispList[i].anchors.top = markerItem.bottom
                    dataDispList[i].anchors.topMargin = 10
                    dataDispList[i]. anchors.horizontalCenter = markerItem.horizontalCenter
                    dataDispList[i].z = markerItem.z + 1
                    _map.addMapItem(markerItem)
                    coordsMarkerList.push(markerItem)
                    //                    console.log(markerItem.coordinate)
                }




            }//for loop ends

        }
        else{
            return
        }


        enableSpinbox = true
        _map.center = coordsListtoDisplay[0]
        coordsAdded()
    }

    function clearMarkers(){
        for(var i=0;i<coordsMarkerList.length;i++){
            _map.removeMapItem(coordsMarkerList[i])
        }
        coordsMarkerList = []
    }

    function clearMapItems(){
        for(var i=0;i<coordsMarkerList.length;i++){
            _map.removeMapItem(coordsMarkerList[i])
        }

        if(finalCoordMarkerList.length > 0){
            for(var j=0; j< finalCoordMarkerList.length;j++){
                var dummy = []
                dummy = finalCoordMarkerList[j]
                var dummy1 = []
                dummy1 = finalDataDispList[j]
                for(var k=0;k<dummy.length;k++){
                    _map.removeMapItem(dummy[k])
                    _map.removeMapItem(dummy1[k])
                }
            }
        }
        if(lineMarkersList.length > 0){
            for(var l=0;l< lineMarkersList.length;l++){
                _map.removeMapItem(lineMarkersList[l])
                _map.removeMapItem(fileNameDataDisplayList[l])
            }
        }


        coordsListtoDisplay = []
        coordsMarkerList = []
        finalCoordMarkerList = []
        finalDataDispList = []
        lineMarkersList = []
        colorList = []
        fileNameDataDisplayList = []

    }

    function changeMarkerSize(size){
        for(var i=0;i<coordsMarkerList.length;i++){
            coordsMarkerList[i].imageSize = size
        }

        //        clearMarkers()
        //        for(var i=0;i< coordsListtoDisplay.length;i++){
        //            var component = Qt.createComponent("MarkerItem.qml");
        //            if (component.status === Component.Ready) {
        //                var markerItem = component.createObject(_map);
        //                markerItem.coordinate =  coordsListtoDisplay[i]
        //                markerItem.index = i
        //                markerItem.imageSize = size
        //                _map.addMapItem(markerItem)
        //                coordsMarkerList.push(markerItem)
        //            }
        //        }


        //        _map.center = coordsListtoDisplay[0]
        //        coordsAdded()

    }

    function dispCoords(coordsList,imageList,coordsTextList){


        var component = Qt.createComponent("MapLines.qml");
        if (component.status === Component.Ready) {
            var contourItem = component.createObject(_map);
            contourItem.col = Qt.rgba(Math.random(),Math.random(),Math.random(),1);

            contourItem.pathList = coordsList
            _map.addMapItem(contourItem)
            lineMarkersList.push(contourItem)
        }
        _map.center = coordsList[0]
        var dataDisplayList = []
        var cMarkList = []

        for(var i=0;i< imageList.length;i++){
            var comp = Qt.createComponent("DisplayRect.qml");
            if (comp.status === Component.Ready) {
                var item = comp.createObject(_map);
                item.imageName = imageList[i]
                item.coord = coordsTextList[i]
                dataDisplayList.push(item)
            }

            var markerComponent = Qt.createComponent("MarkerItem.qml");
            if (markerComponent.status === Component.Ready) {
                var markerItem = markerComponent.createObject(_map);
                markerItem.coordinate =  coordsList[i]
                markerItem.index = i
                markerItem.imageSize = 10
                markerItem.dispRect =dataDisplayList[i]
                dataDisplayList[i].anchors.top = markerItem.bottom
                dataDisplayList[i].anchors.topMargin = 10
                dataDisplayList[i]. anchors.horizontalCenter = markerItem.horizontalCenter
                dataDisplayList[i].z = markerItem.z + 1
                _map.addMapItem(markerItem)
                cMarkList.push(markerItem)
                //                    console.log(markerItem.coordinate)
            }
        }//for loop

        finalDataDispList.push(dataDisplayList)
        finalCoordMarkerList.push(cMarkList)

        displayFileName()
    }

    function displayFileName(){
        if(filesList.length == lineMarkersList.length){
            for(var i=0;i<filesList.length;i++){
                //                console.log(filesList[i],lineMarkersList[i].col,centerCoordsList[i])
                var comp = Qt.createComponent("FileNameDisplay.qml");
                if (comp.status === Component.Ready) {
                    var item = comp.createObject(_map);
                    item.fileNametoDisp = filesList[i]
                    item.coordinate = centerCoordsList[i]
                    fileNameDataDisplayList.push(item)
                    _map.addMapItem(item)
                }

            }
        }
    }


    //for atc


    function getCoordinateFromPt(x,y){
        return _map.toCoordinate(Qt.point(x,y))
    }


    function dispUAVsFromList(){
        for(var i=0; i< uavList.length;i++){
            var comp = Qt.createComponent("VehicleIcon.qml");
            var comp1 = Qt.createComponent("VehicleData.qml");
            var lineComp = Qt.createComponent("MapLines.qml");
            var landComp = Qt.createComponent("LandingItem.qml");
            if (comp.status === Component.Ready) {
                var item = comp.createObject(_map);
                item.imagePath = uavList[i].iconAddress
                item.coordinate = uavList[i].currentLocn
                item.z = 5
                vehicleIconList.push(item)
                _map.addMapItem(item)

                var item1 = comp1.createObject(_map);
                item1.anchors.top = item.bottom
                item1.anchors.horizontalCenter = item.horizontalCenter
                //                item1.verticalCenterOffset = item1.verticalCenterOffset
                item1.uavid = uavList[i].ID
                _map.addMapItem(item1)
                idMarkersList.push(item1)

                var lineItem = lineComp.createObject(_map);
                lineItem.col = Qt.rgba(Math.random(),Math.random(),Math.random(),1);
                lineItem.pathList = uavList[i].flightPath
                _map.addMapItem(lineItem)
                pathMarkersList.push(lineItem)


                var landItem = landComp.createObject(_map);
                landItem.uavID = uavList[i].ID
                landItem.coordinate = uavList[i].destnLocn
                _map.addMapItem(landItem)
                landMarkersList.push(landItem)
            }


        }

    }

    function vehDataChanged(coord){
        vehicleIconList[0].coordinate = coord
    }

    Component.onCompleted: {
        for(var i=0;i < _flightMap.supportedMapTypes.length;i++){
            maptypeListName[i] = _flightMap.supportedMapTypes[i].name

        }
        _flightMap.activeMapType =_flightMap.supportedMapTypes[6]
        _activemapType = _flightMap.activeMapType.name
    }
}
