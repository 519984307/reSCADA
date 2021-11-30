import QtQuick 2.12
import "fap.js" as Fap

Item {
    id: root
    width: 100
    height: 50
    visible: true
    //antialiasing: true
    property int borderWidth: 2
    property string name: "unknown transporter"

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

    property alias rotate: root.rotation
    property int canvasOffset: 3

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
           onTriggered: root.alarmBlink = !root.alarmBlink
    }

    Canvas {
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                root.alarmNotify = false;
                if(mouse.modifiers & Qt.ShiftModifier){
                    root.addToCurrentRoteStoped()
                }else if(mouse.modifiers & Qt.ControlModifier){
                    root.addToCurrentRoteStarted()
                }else if(mouse.button & Qt.RightButton) {
                    if(prop.visible==false){
                        var absolutePos = Fap.getAbsolutePosition(canvas);
                        prop.setX(absolutePos.x+mouseX)
                        prop.setY(absolutePos.y+mouseY)
                        prop.show()
                    }
                    prop.requestActivate()
                }else{
                    if (st){
                        root.stop()
                    }else{
                        root.start()
                    }
                }
            }
            onPressAndHold: {
               root.addToCurrentRoteStarted()
            }
            onDoubleClicked: {
                if(prop.visible==false){
                    var absolutePos = Fap.getAbsolutePosition(canvas);
                    prop.setX(absolutePos.x+mouseX)
                    prop.setY(absolutePos.y+mouseY)
                    prop.show()
                }
            }
        }
        id: canvas
        width: parent.width+canvasOffset*2
        height: parent.height+canvasOffset*2
        x: -canvasOffset
        y: -canvasOffset
        visible: parent.visible
        onPaint: {
            Fap.changeColor(root);
            var context = getContext("2d");
            context.clearRect(0,0,width,height);
            context.strokeStyle = borderColor;
            context.fillStyle = fillColor
            context.lineWidth = borderWidth*2;
            context.beginPath();
            context.moveTo(canvasOffset, canvasOffset);
            context.lineTo(width-canvasOffset, canvasOffset);
            context.lineTo(width-canvasOffset, height-canvasOffset);
            context.lineTo(canvasOffset, height-canvasOffset);
            context.lineTo(canvasOffset, canvasOffset)
            context.closePath;
            context.stroke();
            context.fill();
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
            anchors.rightMargin: 4
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2
            font.pixelSize: 10
            color: Fap.colorArr[prop.route%147].fontColor
        }
        Text{
            id: currentText
            text: prop.current>=0?prop.current:""
            anchors.left: parent.left
            anchors.leftMargin: 4
            anchors.top: parent.top
            anchors.topMargin: 3
            font.pixelSize: 10
            font.family: "DSEG7 Classic"

        }
    }
    Text{
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: name
        visible: parent.visible
    }

    signal speedChanged(variant value)
    function setSpeed(value) {
        speed.setLableSilent(value)
    }

    MFUnit {
        id: speed
        RegExpValidator {
            id: floatreg
            regExp: /[0-9]+[.][0-9]/
        }
        name: "Частота ЧП (Гц)"
        x: 2
        y: 27
        width: 100
        height: 30
        text: ""
        upLimit: 100
        readOnly: false
        regexp: floatreg
        tooltip: "Частота частотного преобразователя транспортёра " + name + " (Гц)"
        //fontSize: 10
        correctingButtons: true
        backgroundColor: "#5fd5fe"
        borderWidth: 1
        onValueChanged: root.speedChanged(value)
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
//    id: root
//    width: 100
//    height: 50
//    visible: true
//    property int borderWidth: 2
//    property string name: "unknown transporter"

//    property color fillColor: "green"
//    property color borderColor: "black"

//    property alias rotate: root.rotation
//    property int canvasOffset: 3

//    Canvas {
//        id: canvas
//        width: parent.width+canvasOffset*2
//        height: parent.height+canvasOffset*2
//        x: -canvasOffset
//        y: -canvasOffset
//        visible: parent.visible
//        onPaint: {
//            //Fap.changeColor(root);
//            var context = getContext("2d");
//            context.strokeStyle = borderColor;
//            context.fillStyle = fillColor
//            context.lineWidth = borderWidth*2;
//            context.beginPath();
//            context.moveTo(canvasOffset, canvasOffset);
//            context.lineTo(width-canvasOffset, canvasOffset);
//            context.lineTo(width-canvasOffset, height-canvasOffset);
//            context.lineTo(canvasOffset, height-canvasOffset);
//            context.lineTo(canvasOffset, canvasOffset)
//            context.closePath;
//            context.stroke();
//            context.fill();
//        }
//    }
//    Text{
//        x: 0
//        y: 0
//        width: parent.width
//        height: parent.height
//        horizontalAlignment: Text.AlignHCenter
//        verticalAlignment: Text.AlignVCenter
//        text: name
//        visible: parent.visible
//    }
//}

///////////////////////////////////////////////////////////////////
///////////////////////////DESIGNER////////////////////////////////
///////////////////////////////////////////////////////////////////
