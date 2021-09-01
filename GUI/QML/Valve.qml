import QtQuick 2.12
import "fap.js" as Fap

Item {
    id: root
    width: 32
    height: 42
    visible: true
    property int borderWidth: 2
    property int canvasOffset: 10
    property string name: "unknown valve"
    property int rotate: 0

    property bool inSt1: false
    property bool st1: false
    property bool std1: false
    property bool inSt2: false
    property bool st2: false
    property bool std2: false
    property bool alarm: prop.alarm
    property bool quitAlarm: false
    property bool blocked: false
    property bool available: prop.available
    property bool linked: prop.linked
    property bool manual1: false
    property bool manual2: false

    property color fillColor1: Fap.defaultColor
    property color borderColor1: Fap.notAlarm
    property color fillColor2: Fap.defaultColor
    property color borderColor2: Fap.notAlarm

    signal start1()
    signal start2()
    signal stop()
    signal resetAlarm()
    signal addToCurrentRoteIn1pos()
    signal addToCurrentRoteIn2pos()

    function pos1(){
        inSt1=true
        inSt2=false
    }

    function pos2(){
        inSt1=false
        inSt2=true
    }

    function posMid(){
        inSt1=false
        inSt2=false
    }

    function posUndef(){
        inSt1=true
        inSt2=true
    }

    function started1(){
        std1=true
        manual1=false
        manual2=false
        startComand1()
    }

    function startComand1(){
        st1=true
        manual1=false
        manual2=false
    }
    function started2(){
        std2=true
        manual1=false
        manual2=false
        startComand2()
    }
    function startComand2(){
        st2=true
        manual1=false
        manual2=false
    }

    function stopComand(){
        st1=false
        st2=false
        manual1=false
        manual2=false
    }
    function stoped(){
        std1=false
        std2=false
        manual1=false
        manual2=false
        stopComand()
    }
    function manualWork1(){
        manual1=true
    }
    function manualWork2(){
        manual2=true
    }

    function openSettings(){
        if(prop.visible==false){
            var absolutePos = Fap.getAbsolutePosition(root);
            prop.setX(absolutePos.x)
            prop.setY(absolutePos.y)
            prop.show()
        }
        prop.requestActivate()
    }

    onSt1Changed: {canvas1.requestPaint(); canvas2.requestPaint(); prop.st1=st1}
    onStd1Changed: {canvas1.requestPaint(); canvas2.requestPaint();}
    onInSt1Changed: {canvas1.requestPaint(); canvas2.requestPaint(); prop.inSt1=inSt1}
    onSt2Changed: {canvas1.requestPaint(); canvas2.requestPaint(); prop.st2=st2}
    onStd2Changed: {canvas1.requestPaint(); canvas2.requestPaint();}
    onInSt2Changed: {canvas1.requestPaint(); canvas2.requestPaint(); prop.inSt2=inSt2}
    onAlarmChanged: {canvas1.requestPaint(); canvas2.requestPaint();}
    onQuitAlarmChanged: {canvas1.requestPaint(); canvas2.requestPaint();}
    onBlockedChanged: {canvas1.requestPaint(); canvas2.requestPaint();}
    onAvailableChanged: {canvas1.requestPaint(); canvas2.requestPaint();}
    onLinkedChanged: {canvas1.requestPaint(); canvas2.requestPaint();}
    onManual1Changed: {canvas1.requestPaint(); canvas2.requestPaint();}
    onManual2Changed: {canvas1.requestPaint(); canvas2.requestPaint();}

    Item{
        id:valveCanvases
        x:0
        y:0
        width: parent.width
        height: parent.height
        visible: parent.visible
        rotation: rotate
        Canvas {
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    if((mouse.button & Qt.RightButton)&&(mouse.modifiers & Qt.ControlModifier)){

                    }else if((mouse.button & Qt.LeftButton)&&(mouse.modifiers & Qt.ControlModifier)){
                        root.addToCurrentRoteIn1pos()
                    }else if(mouse.button & Qt.RightButton) {
                        root.openSettings()
                    }else{
                        if (st1||st2){
                            root.stop()
                        }else{
                            root.start1();
                        }
                    }
                }
                onPressAndHold: {
                   root.addToCurrentRoteIn1pos()
                }
                onDoubleClicked: {
                    root.openSettings()
                }
            }
            id: canvas1
            width: 15
            height: 25
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            rotation: -90
            visible: parent.visible
            onPaint: {
                Fap.changeColorV(root);
                var context = getContext("2d");
                context.clearRect(0,0,width,height);
                context.beginPath();
                context.fillStyle = fillColor1
                context.strokeStyle = borderColor1
                context.lineWidth = borderWidth*2;
                context.moveTo(0, height/2);
                context.lineTo(width/2, 0);
                context.lineTo(width, height/2);
                context.lineTo(width-width/3, height/2);
                context.lineTo(width-width/3, height);
                context.lineTo(width/3, height);
                context.lineTo(width/3, height/2);
                context.closePath()
                context.stroke();
                context.fill();
            }
        }
        Canvas {
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    if((mouse.button & Qt.RightButton)&&(mouse.modifiers & Qt.ControlModifier)){
                    }else if((mouse.button & Qt.LeftButton)&&(mouse.modifiers & Qt.ControlModifier)){
                        root.addToCurrentRoteIn2pos()
                    }else if(mouse.button & Qt.RightButton) {
                        root.openSettings()
                    }else{
                        if (st1||st2){
                            root.stop()
                        }else{
                            root.start2()
                        }
                    }
                }
                onPressAndHold: {
                   root.addToCurrentRoteIn2pos()
                }
                onDoubleClicked: {
                    root.openSettings()
                }
            }
            id: canvas2
            width: 15
            height: 25
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            rotation: -180
            visible: parent.visible
            onPaint: {
                Fap.changeColorV(root);
                var context = getContext("2d");
                context.clearRect(0,0,width,height);
                context.beginPath();
                context.fillStyle = fillColor2
                context.strokeStyle = borderColor2
                context.lineWidth = borderWidth*2;
                context.moveTo(0, height/2);
                context.lineTo(width/2, 0);
                context.lineTo(width, height/2);
                context.lineTo(width-width/3, height/2);
                context.lineTo(width-width/3, height);
                context.lineTo(width/3, height);
                context.lineTo(width/3, height/2);
                context.closePath()
                context.stroke();
                context.fill();
            }
        }
    }
    Text {
        id: nameText
        y: 21
        text: root.name
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.rightMargin: 15
        anchors.leftMargin: 15
        font.pixelSize: 10
    }

    Component.onCompleted: {
        if(root.rotate==0){
            nameText.anchors.right = root.right
        } else{
            nameText.anchors.left = root.left
        }
    }

    PropertyWindow2 {
        id: prop
        objectName: "prop"
        winTitle: name
        onStart1: root.start1()
        onStart2: root.start2()
        onStop: root.stop()
        st1: root.st1
        st2: root.st2
        inSt1: root.inSt1
        inSt2: root.inSt2
        //onResetAlarm: root.resetAlarm()
        buttonsType: 1
    }
}
