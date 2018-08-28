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

    property var currentCoord
    //atc
    property var uavList: []
    property var vehicleIconList: []
    property var idMarkersList: []
    property var pathMarkersList: []
    property var landMarkersList: []
    property string testCase: ""
    property var dataDisplayList: []
    property var geolandMarkersList: []
    property var dispitemList: []
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
        console.log("dispUAVsFromList")
        for(var i=0; i< uavList.length;i++){
            var comp = Qt.createComponent("VehicleIcon.qml");
            var comp1 = Qt.createComponent("VehicleData.qml");
            var lineComp = Qt.createComponent("MapLines.qml");
            var landComp = Qt.createComponent("LandingItem.qml");
            if (comp.status === Component.Ready) {
                var item = comp.createObject(_map);
                item.imagePath = uavList[i].iconAddress
                item.visible  = true
                item.coordinate = uavList[i].homeLocn
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

        if(testCase == "GeoFence Breach"){

            displayGeoFence();
        }
        else if(testCase == "Mid-Air Collision"){
        }

    }

    function vehDataChanged(coord){
        vehicleIconList[0].coordinate = coord
        currentCoord = coord
    }


    function vehDataChangedList(coord,i){
        vehicleIconList[i].coordinate = coord

    }



    function collisionPossible(i,j){

        console.log(i,j,"collisionPossible")
    }

    function dispAvoidanceRect(){
        var rectcomp = Qt.createComponent("DisplayRect.qml");
        var dispitem;
        if (rectcomp.status === Component.Ready) {
            dispitem = rectcomp.createObject(_map);

        }

        dispitem.height = 75
        dispitem.width = 800
        dispitem.coord =  "D2 is made to loiter and give way for D1 based on priority.\n Different overrides like RTL/Loiter can be performed based on the uav capability"
        dispitem.visible =true
        _map.addMapItem(dispitem)
        dispitem.anchors.top = landMarkersList[1].bottom
        dispitem.anchors.topMargin = 300
        dispitem. anchors.horizontalCenter = vehicleIconList[1].horizontalCenter
        dispitem.z = vehicleIconList[1].z + 1
    }

    function displayGeoFence(){

        _geoFence.center = uavList[0].homeLocn
        _geoFence.radius = 500
        _geoFence.visible = true

    }

    function displayGeoFenceBreach(){
        _geoFence.color = "red"
        _geoFence.border.color = "red"
        dataDisplayList = []
        var comp = Qt.createComponent("DisplayRect.qml");
        var item;
        if (comp.status === Component.Ready) {
            item = comp.createObject(_map);
            //            item.imageName = "Sending Drone to nearest Landing Location L1"
            //            item.coordinate = currentCoord
            item.coord = "Sending Drone to nearest Landing Location L1. \nGeoFence Breached"
            dataDisplayList.push(item)
            item.visible =true
            _map.addMapItem(item)
        }
        // displaying landing locns
        var c1, c2
        c1 = QtPositioning.coordinate(  42.384979,-83.045731)
        c2 = QtPositioning.coordinate(  42.384624,-83.043696)
        var landComp = Qt.createComponent("LandingItem.qml");
        var landComp1 = Qt.createComponent("LandingItem.qml");
        var item1 ,item2;
        if (landComp.status === Component.Ready) {
            item1 = landComp.createObject(_map);

            item2 = landComp1.createObject(_map);
            item1.uavID = "L1"
            item2.uavID = "L2"
            item1.coordinate = c1
            item2.coordinate = c2
            item1.visible = true
            item2.visible = true
            _map.addMapItem(item1)
            _map.addMapItem(item2)
        }
        dataDisplayList[0].anchors.top = item1.bottom
        dataDisplayList[0].anchors.topMargin = 10
        dataDisplayList[0]. anchors.horizontalCenter = item1.horizontalCenter
        dataDisplayList[0].z = item1.z + 1
        geolandMarkersList.push(item1)
        geolandMarkersList.push(item2)
        _controlWindow.sendUAVToLocn(c1)

    }

    function displayGeoFenceAfterBreach(){
        _geoFence.color = "transparent"
        _geoFence.border.color = "#00ADEF"
    }


    function dispNoFlyZoneTestCase(flightPlan,zoneCoords,plan2,plan3){

        showFlightPlan(flightPlan,"1")
        showNoFlyZone(zoneCoords)
        showFlightPlan(plan2,"2")
        showFlightPlan(plan3,"3")
    }


    function clearGeofenceBreach(){

        _geoFence.visible = false
        //        _geoFence.path = []
        for(var i=0; i<dataDisplayList.length;i++){
            _map.removeMapItem(dataDisplayList[i])
        }
        for(var j=0; j<geolandMarkersList.length;j++){
            _map.removeMapItem(geolandMarkersList[j])
        }
        dataDisplayList = []
        geolandMarkersList = []
    }

    function clearNoflyZone(){

        _noFlyZone.visible = false
        _noFlyZone.path = []
        _noflyzonetitlerect.visible = false
        for(var i=0;i<lineMarkersList.length;i++){
            _map.removeMapItem(lineMarkersList[i])
        }
        lineMarkersList = []
        dispitemList = []
    }

    function clearAvoidance() {

    }

    function clearallitems(){
        for(var i=0;i<vehicleIconList.length;i++){
            _map.removeMapItem(vehicleIconList[i])
        }
        for(var j=0;j<idMarkersList.length;j++){
            _map.removeMapItem(idMarkersList[j])
        }
        for(var k=0;k<pathMarkersList.length;k++){
            _map.removeMapItem(pathMarkersList[k])
        }
        for(var l=0;l<landMarkersList.length;l++){
            _map.removeMapItem(landMarkersList[l])
        }
        vehicleIconList = []
        idMarkersList= []
        pathMarkersList = []
        landMarkersList = []

        clearGeofenceBreach()
        clearNoflyZone()
        clearAvoidance()
    }

    function showFlightPlan(flightPlan,index){
        var component = Qt.createComponent("MapLines.qml");
        var comp = Qt.createComponent("LandingItem.qml");
        if (component.status === Component.Ready) {
            var contourItem = component.createObject(_map);
            contourItem.col = Qt.rgba(Math.random(),Math.random(),Math.random(),1);

            contourItem.pathList = flightPlan
            _map.addMapItem(contourItem)
            lineMarkersList.push(contourItem)
            var litem = comp.createObject(_map);
            //            console.log(flightPlan.last())
            litem.coordinate = flightPlan[flightPlan.length -1]
            litem.uavID = "D" + index
            _map.addMapItem(litem)
            var rectcomp = Qt.createComponent("DisplayRect.qml");
            var dispitem;
            if (rectcomp.status === Component.Ready) {
                dispitem = rectcomp.createObject(_map);

            }

            if(index === "1"){
                dispitem.height = 75
                dispitem.coord = litem.uavID + " Flight plan Rejected. \nReason: Flight plan lies in a No-Fly Zone."
                dispitem.visible =true
                _map.addMapItem(dispitem)
            }
            else{
                dispitem.coord =litem.uavID + " Flight plan accepted"
                dispitem.visible =true
                _map.addMapItem(dispitem)
            }
            dispitem.anchors.top = litem.bottom
            dispitem.anchors.topMargin = 10
            dispitem. anchors.horizontalCenter = litem.horizontalCenter
            dispitem.z = litem.z + 1
        }
        dispitemList.push(dispitem)

    }


    function showNoFlyZone(zoneCoords){
        _noFlyZone.path = zoneCoords
        _noFlyZone.visible = true



    }

    MapCircle{
        id:_geoFence
        visible: false
        color: "transparent"
        border.color: "#00ADEF"
        border.width: 20
        opacity: 0.3

    }

    MapPolygon{
        id:_noFlyZone
        visible: false
        border.color: "red"
        border.width: 5
        z: 5
        color: "transparent"
    }

    Component.onCompleted: {
        for(var i=0;i < _flightMap.supportedMapTypes.length;i++){
            maptypeListName[i] = _flightMap.supportedMapTypes[i].name

        }
        _flightMap.activeMapType =_flightMap.supportedMapTypes[6]
        _activemapType = _flightMap.activeMapType.name
    }
}
