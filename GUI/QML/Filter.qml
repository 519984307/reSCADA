import QtQuick 2.12
import "fap.js" as Fap

Item {
    id: root
    width: 100
    height: 100
    visible: true
    property int borderWidth: 2
    property string name: "unknown filter"
    property bool mirror: false

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

    onStChanged: {canvas.requestPaint(); prop.st=st}
    onStdChanged: canvas.requestPaint()
    onAlarmChanged: canvas.requestPaint()
    onAlarmNotifyChanged: {alarmNotify?timer.start():timer.stop()}
    onAlarmBlinkChanged: canvas.requestPaint();
    onBlockedChanged: canvas.requestPaint()
    onAvailableChanged: canvas.requestPaint()
    onLinkedChanged: canvas.requestPaint()
    onManualChanged: canvas.requestPaint()

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
        width: parent.width
        height: parent.height
        x: 0
        y: 0
        visible: parent.visible
        onPaint: {
            Fap.changeColor(root);
            var context = getContext("2d");
            context.clearRect(0,0,width,height);
            context.strokeStyle = borderColor;
            context.fillStyle = fillColor
            context.lineWidth = borderWidth*2;
            context.beginPath();
            if(mirror){
                context.moveTo(0.9*width, 0.1*height);
                context.lineTo(0.1*width, 0.1*height);
                context.lineTo(0.1*width, 0.7*height);
                context.lineTo(0.9*width, 0.9*height);
            }else{
                context.moveTo(0.1*width, 0.1*height);
                context.lineTo(0.9*width, 0.1*height);
                context.lineTo(0.9*width, 0.7*height);
                context.lineTo(0.1*width, 0.9*height);
            }
            context.closePath;
            context.stroke();
            context.fill();
        }

    }
    Text{
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        rotation: -90
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: name
        font.pixelSize: 10
        visible: parent.visible
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
