import QtQuick 2.12
import "fap.js" as Fap

Item {
    width: 68
    height: 260
    visible: true
    id: root
    property bool upLeft: true
    property bool upRight: false
    property bool downLeft: false
    property bool downRight: true
    property int heighIndentation: 15
    property int widthIndentation: 15
    property int elevatorThin: width/2.8
    property int borderWidth: 2
    property int canvasOffset: 2
    property string name: "unknown elevator"
    property alias fontSize: nameText.font.pixelSize
    property bool st: false
    property bool std: false
    property alias alarm: prop.alarm
    property alias alarmNotify: prop.alarmNotify
    property bool alarmBlink: false
    property bool blocked: false
    property bool available: prop.available
    property bool linked: prop.linked
    property bool manual: false
    property bool cleaning: false

    property color fillColor: Fap.defaultColor
    property color borderColor: Fap.notAlarm

    signal start()
    signal stop()
    signal resetAlarm()
    signal addToCurrentRoteStarted()
    signal addToCurrentRoteStoped()

    function started(){
        startComand()
        std=true
        manual=false
        cleaning=false
    }
    function stoped(){
        stopComand()
        std=false
        manual=false
        cleaning=false
    }
    function startComand(){
        st=true
        manual=false
        cleaning=false
    }
    function stopComand(){
        st=false
        manual=false
        cleaning=false
    }
    function manualWork(){
        manual=true
    }

    function cleaningWork(){
        cleaning=true
    }
    function openSettings(){
        if(prop.visible==false){
            var absolutePos = Fap.getAbsolutePosition(canvas);
            prop.setX(absolutePos.x)
            prop.setY(absolutePos.y)
            prop.show()
        }
        prop.requestActivate()
    }

    onStChanged: {canvas.requestPaint(); prop.st=st}
    onStdChanged: canvas.requestPaint()
    onAlarmChanged: canvas.requestPaint()
    onAlarmNotifyChanged: {alarmNotify?timer.start():timer.stop(); canvas.requestPaint()}
    onAlarmBlinkChanged: canvas.requestPaint();
    onBlockedChanged: canvas.requestPaint()
    onAvailableChanged: canvas.requestPaint()
    onLinkedChanged: canvas.requestPaint()
    onManualChanged: canvas.requestPaint()
    onCleaningChanged: canvas.requestPaint()

    Timer {
        id: timer
        interval: Fap.blinkDelay; running: false; repeat: true
        onTriggered: {
            root.alarmBlink = !root.alarmBlink
        }
    }

    Canvas {
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if (!Fap.isInArea(canvas.points, canvas.size, {X: mouseX, Y: mouseY})) return;
                root.alarmNotify = false;
                if(mouse.modifiers & Qt.ShiftModifier){
                    root.addToCurrentRoteStoped()
                }else if(mouse.modifiers & Qt.ControlModifier){
                    root.addToCurrentRoteStarted()
                }else if(mouse.button & Qt.RightButton) {
                    root.openSettings()
                }else{
                    if (st){
                        root.stop()
                    }else{
                        root.start()
                    }
                }
            }
            onPressAndHold: {
                if (!Fap.isInArea(canvas.points, canvas.size, {X: mouseX, Y: mouseY})) return;
                root.addToCurrentRoteStarted()
            }
            onDoubleClicked: {
                if (!Fap.isInArea(canvas.points, canvas.size, {X: mouseX, Y: mouseY})) return;
                root.openSettings()
            }
        }
        property variant points: []
        property int size: 0
        id: canvas
        width: parent.width+canvasOffset*2
        height: parent.height+canvasOffset*2
        x: -canvasOffset
        y: -canvasOffset
        visible: parent.visible
        onPaint: {
            var p = []
            Fap.changeColor(root);
            var context = getContext("2d");
            context.clearRect(0,0,width,height);
            context.beginPath();
            context.fillStyle = fillColor
            context.strokeStyle = borderColor
            context.lineWidth = borderWidth*2;
            if(upLeft){
                context.moveTo(canvasOffset+widthIndentation, canvasOffset); p.push({X:canvasOffset+widthIndentation, Y:canvasOffset});
            }else{
                context.moveTo(canvasOffset+elevatorThin, canvasOffset); p.push({X:canvasOffset+elevatorThin, Y:canvasOffset});
            }
            if (upRight){
                context.lineTo(width-canvasOffset-widthIndentation, canvasOffset); p.push({X:width-canvasOffset-widthIndentation, Y:canvasOffset});
                context.lineTo(width-canvasOffset, canvasOffset+heighIndentation); p.push({X:width-canvasOffset, Y:canvasOffset+heighIndentation});
                context.lineTo(width-canvasOffset-elevatorThin, canvasOffset+heighIndentation); p.push({X:width-canvasOffset-elevatorThin, Y:canvasOffset+heighIndentation});
            }else{
                context.lineTo(width-canvasOffset-elevatorThin, canvasOffset); p.push({X:width-canvasOffset-elevatorThin, Y:canvasOffset});
            }
            if (downRight){
                context.lineTo(width-canvasOffset-elevatorThin, height-canvasOffset-heighIndentation); p.push({X:width-canvasOffset-elevatorThin, Y:height-canvasOffset-heighIndentation});
                context.lineTo(width-canvasOffset, height-canvasOffset-heighIndentation); p.push({X:width-canvasOffset, Y:height-canvasOffset-heighIndentation});
                context.lineTo(width-canvasOffset-widthIndentation, height-canvasOffset); p.push({X:width-canvasOffset-widthIndentation, Y:height-canvasOffset});
            }else{
                context.lineTo(width-canvasOffset-elevatorThin, height-canvasOffset); p.push({X:width-canvasOffset-elevatorThin, Y:height-canvasOffset});
            }
            if (downLeft){
                context.lineTo(canvasOffset+widthIndentation, height-canvasOffset); p.push({X:canvasOffset+widthIndentation, Y:height-canvasOffset});
                context.lineTo(canvasOffset, height-canvasOffset-heighIndentation); p.push({X:canvasOffset, Y:height-canvasOffset-heighIndentation});
                context.lineTo(canvasOffset+elevatorThin, height-canvasOffset-heighIndentation); p.push({X:canvasOffset+elevatorThin, Y:height-canvasOffset-heighIndentation});
            }else{
                context.lineTo(canvasOffset+elevatorThin, height-canvasOffset); p.push({X:canvasOffset+elevatorThin, Y:height-canvasOffset});
            }
            if (upLeft){
                context.lineTo(canvasOffset+elevatorThin, canvasOffset+heighIndentation); p.push({X:canvasOffset+elevatorThin, Y:canvasOffset+heighIndentation});
                context.lineTo(canvasOffset, canvasOffset+heighIndentation); p.push({X:canvasOffset, Y:canvasOffset+heighIndentation});
                context.closePath()
            }else{
                context.closePath()
            }
            context.closePath()
            context.stroke();
            context.fill();
            canvas.points = p;
            canvas.size = p.length;
        }
    }

    Rectangle{
        anchors.fill: routeText
        anchors.leftMargin: -3
        anchors.rightMargin: -3
        border.width: 1
        border.color: "black"
        color: Fap.colorArr[prop.route%147].backColor
        visible: prop.route>0
    }
    Text{
        id: routeText
        text: prop.route>0?prop.route:""
        anchors.right: parent.right
        anchors.rightMargin: root.downRight?root.widthIndentation+2:(root.width-root.elevatorThin)/2+6
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 3
        font.pixelSize: 10
        color: Fap.colorArr[prop.route%147].fontColor
    }

    Text{
        id: currentText
        text: prop.current>=0?prop.current:""
        anchors.left: parent.left
        anchors.leftMargin: root.upLeft?root.widthIndentation+2:(root.width-root.elevatorThin)/2+6
        anchors.top: parent.top
        anchors.topMargin: 3
        font.pixelSize: 10
        font.family: "DSEG7 Classic"
    }
    Text {
        id: nameText
        x: parent.width/2
        y: parent.height/2
        text: name
        enabled: true
        smooth: false
        antialiasing: true
        rotation: -90
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 12
    }

    PropertyWindow2 {
        id: prop
        objectName: "prop"
        winTitle: name
        onStart: root.start()
        onStop: root.stop()
        //onResetAlarm: root.resetAlarm()
        buttonsType: 0
    }
}


///////////////////////////////////////////////////////////////////
///////////////////////////DESIGNER////////////////////////////////
///////////////////////////////////////////////////////////////////

//import QtQuick 2.12

//Item {
//    width: 68
//    height: 260
//    visible: true
//    id: root
//    property bool upLeft: true
//    property bool upRight: false
//    property bool downLeft: false
//    property bool downRight: true
//    property int heighIndentation: 15
//    property int widthIndentation: 15
//    property int elevatorThin: width/2.8
//    property int borderWidth: 2
//    property int canvasOffset: 2
//    property string name: "unknown elevator"

//    property color fillColor: "green"
//    property color borderColor: "black"

//    Canvas {
//        id: canvas
//        width: parent.width+canvasOffset*2
//        height: parent.height+canvasOffset*2
//        x: -canvasOffset
//        y: -canvasOffset
//        visible: parent.visible
//        onPaint: {
//            var context = getContext("2d");
//            context.beginPath();
//            context.fillStyle = fillColor
//            context.strokeStyle = borderColor
//            context.lineWidth = borderWidth*2;
//            if(upLeft){
//                context.moveTo(canvasOffset+widthIndentation, canvasOffset);
//            }else{
//                context.moveTo(canvasOffset+elevatorThin, canvasOffset);
//            }
//            if (upRight){
//                context.lineTo(width-canvasOffset-widthIndentation, canvasOffset);
//                context.lineTo(width-canvasOffset, canvasOffset+heighIndentation);
//                context.lineTo(width-canvasOffset-elevatorThin, canvasOffset+heighIndentation);
//            }else{
//                context.lineTo(width-canvasOffset-elevatorThin, canvasOffset);
//            }
//            if (downRight){
//                context.lineTo(width-canvasOffset-elevatorThin, height-canvasOffset-heighIndentation);
//                context.lineTo(width-canvasOffset, height-canvasOffset-heighIndentation);
//                context.lineTo(width-canvasOffset-widthIndentation, height-canvasOffset);
//            }else{
//                context.lineTo(width-canvasOffset-elevatorThin, height-canvasOffset);
//            }
//            if (downLeft){
//                context.lineTo(canvasOffset+widthIndentation, height-canvasOffset);
//                context.lineTo(canvasOffset, height-canvasOffset-heighIndentation);
//                context.lineTo(canvasOffset+elevatorThin, height-canvasOffset-heighIndentation);
//            }else{
//                context.lineTo(canvasOffset+elevatorThin, height-canvasOffset);
//            }
//            if (upLeft){
//                context.lineTo(canvasOffset+elevatorThin, canvasOffset+heighIndentation);
//                context.lineTo(canvasOffset, canvasOffset+heighIndentation);
//                context.closePath()
//            }else{
//                context.closePath()
//            }
//            context.closePath()
//            context.stroke();
//            context.fill();
//        }
//        Text {
//            id: element
//            x: parent.width/2
//            y: parent.height/2
//            text: name
//            enabled: true
//            smooth: false
//            antialiasing: true
//            rotation: -90
//            anchors.horizontalCenterOffset: 0
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.verticalCenter: parent.verticalCenter
//            font.pixelSize: 12
//        }
//    }
//}

///////////////////////////////////////////////////////////////////
///////////////////////////DESIGNER////////////////////////////////
///////////////////////////////////////////////////////////////////
