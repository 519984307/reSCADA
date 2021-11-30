import QtQuick 2.12
import QtQuick.Controls 2.12
import "fap.js" as Fap

Rectangle {
    id: root
    visible: true
    color: Fap.contrBackground
    border.width: 1
    border.color: "black"
    property int rowSize: 25
    property int counter: 0
    property color setClr: "#ace1e6"
    property int buttonPixelSize: 17
    property int routePixelSize: 10
    //INPUT
    function appendRoute(id){
        counter ++
        model.append({name: id.toString(), st: false, symName: "Маршрут "+id.toString(), routestr: "", current: false, colorChange: (counter % 2) })
        setRouteCurrent(id)
    }
    function startedRoute(id){
        var object
        var i = 0
        do{
            object = model.get(i)
            if(object !== undefined){
                if(object.name===id.toString()){
                    object.st=true
                }
            }
            i++
        }while(object !== undefined)
    }
    function stoppedRoute(id){
        var object
        var i = 0
        do{
            object = model.get(i)
            if(object !== undefined){
                if(object.name===id.toString()){
                    object.st=false
                }
            }
            i++
        }while(object !== undefined)
    }
    function updateRoute(id, routeString){
        var object
        var i = 0
        do{
            object = model.get(i)
            if(object !== undefined){
                if(object.name===id.toString()){
                    object.routestr=routeString
                }
            }
            i++
        }while(object !== undefined)
    }
    function setRouteName(id, routeName){
        var object
        var i = 0
        do{
            object = model.get(i)
            if(object !== undefined){
                if(object.name===id.toString()){
                    object.symName=routeName
                }
            }
            i++
        }while(object !== undefined)
    }


    //OUTPUT
    signal createRoute();
    signal deleteRoute(int id)
    signal startRoute(int id);
    signal cleanRoute(int id);
    signal stopRoute(int id);
    signal deleteRouteLastUnit(int id);
    signal setCurrentRoute(int id);
    signal updateRouteName(int id, string routeName);

    //INTERNAL
    function setRouteCurrent( id ){
        var object
        var i = 0
        do{
            object = model.get( i )
            if(object !== undefined){
                if(object.name===id.toString()){
                    object.current=true
                }else{
                    object.current=false
                }
            }
            i++
        }while(object !== undefined)
        setCurrentRoute(id)
    }
    function deleteRouteInside(id, index){
        counter --
        model.remove(index)
        deleteRoute(id)
        var object = model.get(0)
        if(object !== undefined){
            setRouteCurrent(parseInt(object.name, 10))
        }
        for(let i = 0; i < model.count; i ++){
            model.setProperty(i, "colorChange", i % 2);
        }
    }

    // parseInt(name, 10)

    Component{
        id: component
        Rectangle {
            id: route
            property bool active: false
            property real symNameRatio: 0.15
            property int pixelSize: root.routePixelSize
            width: parent.width
            height: rowSize
            color: colorChange == 1 ? "#ace1e6" : "white"
            Text{
                id: routeId
                x: 0
                y: 0
                height: rowSize
                width: rowSize
                text: name+"."
                font.pixelSize: route.pixelSize
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                renderType: Text.NativeRendering
            }
            Row {
                id: buttons
                x: routeId.width//parent.width-width
                y: 0
                height: parent.height
                width: 144

                Button{
                    id: startButton
                    height: parent.height
                    width: parent.width/6
                    Text{
                        anchors.fill: parent
                        text: "1"
                        color: routestr===""?"gray":(st?"green":"black")
                        font.pixelSize: root.buttonPixelSize
                        font.family: "Guifx v2 Transports"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        if(routestr!==""){
                            root.startRoute(parseInt(name, 10))
                        }
                    }
                }
                Button{
                    id: stopButton
                    height: parent.height
                    width: parent.width/6
                    Text{
                        anchors.fill: parent
                        text: "3"
                        //color: route.st?"black":"red"
                        font.pixelSize: root.buttonPixelSize
                        font.family: "Guifx v2 Transports"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: root.stopRoute(parseInt(name, 10))
                }
                Button{
                    id: cleanButton
                    height: parent.height
                    width: parent.width/6
                    Text{
                        anchors.fill: parent
                        text: "c"
                        //color: route.st?"black":"gray"
                        font.pixelSize: root.buttonPixelSize
                        font.family: "Guifx v2 Transports"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: root.cleanRoute(parseInt(name, 10))
                }
                Button{
                    id: addButton
                    height: parent.height
                    width: parent.width/6
                    Text{
                        anchors.fill: parent
                        text: "="
                        color: current?"blue":"black"
                        font.pixelSize: root.buttonPixelSize
                        font.family: "Guifx v2 Transports"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: root.setRouteCurrent(parseInt(name, 10))
                }
                Button{
                    id: clearButton
                    height: parent.height
                    width: parent.width/6
                    Text{
                        anchors.fill: parent
                        text: "-"
                        color: (routestr===""||st)?"gray":"black"
                        font.pixelSize: root.buttonPixelSize
                        font.family: "Guifx v2 Transports"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        if((routestr!=="")&&!st){
                            root.deleteRouteLastUnit(parseInt(name, 10))
                        }
                    }
                }
                Button{
                    id: deleteButton
                    height: parent.height
                    width: parent.width/6
                    Text{
                        anchors.fill: parent
                        text: "x"
                        color: st?"gray":"black"
                        font.pixelSize: root.buttonPixelSize
                        font.family: "Guifx v2 Transports"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                    }
                    onClicked: {
                        if(!st){
                            root.deleteRouteInside(parseInt(name, 10), index)
                        }
                    }
                }
            }
            Item{
                id: routeSymName
                x:  buttons.x + buttons.width + 4
                y: 0
                height: rowSize
                width: 300  //parent.width * symNameRatio
                Text {
                    id: routeSymNameShow
                    visible: true
                    x: 0
                    y: 0
                    height: parent.height
                    width: parent.width
                    text: symName
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: route.pixelSize + 1
                    wrapMode: Text.Wrap
                    renderType: Text.NativeRendering
                    MouseArea{
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        onClicked: {
                            routeSymNameShow.visible = false
                            routeSymNameEdit.enabled = true
                            routeSymNameEdit.forceActiveFocus()
                        }
                    }
                }

                TextField{
                    visible: !routeSymNameShow.visible
                    id: routeSymNameEdit
                    x: 0
                    y: 0
                    height: parent.height
                    width: parent.width
                    text: symName
                    font.pixelSize: route.pixelSize
                    clip: true
                    onFocusChanged: {
                        if(!activeFocus){
                            routeSymNameShow.visible = true
                            symName = text
                            updateRouteName(parseInt(name, 10), symName)
                            routeSymNameEdit.enabled = false
                        }
                    }
                    Keys.onPressed:{
                        if (event.key===16777220){ //Enter
                            routeSymNameShow.visible = true
                            symName = text
                            updateRouteName(name, symName)
                            routeSymNameEdit.enabled = false
                        }
                    }
                }
            }
            Text {
                id: routeText
                x: routeSymName.x + routeSymName.width + 4
                renderType: Text.NativeRendering
                y: 0
                height: parent.height
                width: parent.width - x
                clip: true
                font.pixelSize: route.pixelSize
                text: (routestr==="")?"Маршрут не задан":routestr
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
            }

        }
    }

    ListModel{
        id: model
    }

    Item{
        id: routeArea
        width: parent.width
        height: parent.height
        anchors.margins: 4
        Item{
            id: head
            height: 15
            width: parent.width
            Text {
                anchors.fill: parent
                id: title
                text: "Маршруты"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        Item{
            id: body
            width: parent.width
            anchors.top: head.bottom
            anchors.bottom: buttons.top
            ListView{
                id: view
                anchors.fill: parent
                anchors.leftMargin: 4
                anchors.rightMargin: 4
                clip: true
                model: model
                delegate: component
            }
        }
        Item{
            id: buttons
            height: 30
            width: parent.width
            anchors.bottom: parent.bottom
            Button {
                id: buttonAdd
                anchors.fill: parent
                anchors.margins: 4
                text: "Добавить маршрут"
                onClicked: {
                    root.createRoute()
                    view.positionViewAtEnd();
                }
            }
        }
    }
}
