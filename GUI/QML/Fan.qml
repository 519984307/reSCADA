import QtQuick 2.12
import "fap.js" as Fap

Item {
    width: 30
    height: 30
    visible: true
    id: root

    property int borderWidth: 2
    property string name: "unknown fan"
    property bool mount: true
    property bool mirror: false
    property int rotatePict: 0
    property alias fontSize: fanText.font.pointSize
    property alias textX: fanText.x
    property alias textY: fanText.y

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

    Timer {
           id: timer
           interval: Fap.blinkDelay; running: false; repeat: true
           onTriggered: root.alarmBlink = !root.alarmBlink
    }

    Canvas {     
        id: canvas
        anchors.fill: parent
        rotation: rotatePict
        visible: parent.visible
        transform: Scale {origin.x: width / 2; xScale: root.mirror?-1:1; yScale: 1}
        onPaint: {
            Fap.changeColor(root);
            var context = getContext("2d");
            context.clearRect(0,0,width,height);
            context.strokeStyle = borderColor;
            context.fillStyle = fillColor
            context.lineWidth = borderWidth*2;
            context.beginPath();
            context.moveTo(width*0.85, 0);
            context.lineTo(width*0.85, height/2);
            context.lineTo(width*0.6, height/2);
            context.lineTo(width*0.6, 0);
            context.closePath;
            context.stroke();
            context.fill();

            if (mount){
                context.beginPath();
                context.moveTo(width/2, height*0.85);
                context.lineTo(0, height*0.85);
                context.stroke();
            }

            context.beginPath();
            context.moveTo(width/2, height/2);
            context.arc(width/2, height/2, width*0.35, 0, 2*Math.PI, true)
            context.closePath;
            context.stroke();
            context.fill();

        }
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
               root.addToCurrentRoteStarted()
            }
            onDoubleClicked: {
                root.openSettings()
            }
        }
    }
        Text{
            id: fanText
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: name
            font.pointSize: 5
            visible: parent.visible
            wrapMode: Text.Wrap
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

/*##^##
Designer {
    D{i:0;height:55;width:56}D{i:1}D{i:3}D{i:2}D{i:5}D{i:6}
}
##^##*/
