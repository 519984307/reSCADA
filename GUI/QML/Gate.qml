import QtQuick 2.12
import "fap.js" as Fap

Item {
    id: root
    width: 83
    height: 13
    visible: true
    property int borderWidth: 2
    property int textPosition: 2
    property string name: "unkown gate"

    property int canvasOffset: 2

    property color fillColor1:Fap.defaultColor
    property color borderColor1: Fap.notAlarm
    property color fillColor2: Fap.defaultColor
    property color borderColor2: Fap.notAlarm
    property bool opened: false
    property bool openCmd: false
    property bool openMove: false
    property bool closed: false
    property bool closedCmd: false
    property bool closedMove: false
    property bool alarm: prop.alarm
    property bool quitAlarm: false
    property bool blocked: false
    property bool available: prop.available
    property bool linked: prop.linked
    property bool manualOpenMove: false
    property bool manualCloseMove: false
    property alias fontOX: cl.font
    //    QtObject {
//        id:commonParam
//        property font fontCommon: Qt.font({pointSize: 15})
//    }

    signal open()
    signal start1()
    signal close()
    signal start2()
    signal stop()
    signal resetAlarm()
    signal addToCurrentRoteIn1pos()
    signal addToCurrentRoteInOpened()
    signal addToCurrentRoteIn2pos()
    signal addToCurrentRoteInClosed()

    function pos1(){
        opened=true
        closed=false
    }

    function pos2(){
        opened=false
        closed=true
    }

    function posMid(){
        opened=false
        closed=false
    }

    function posUndef(){
        opened=true
        closed=true
    }

    function startedOpen(){
        openMove=true
        startComand1()
    }
    function started1(){
        root.startedOpen()
    }

    function startComandOpen(){
        openCmd=true
        manualOpenMove=false
        manualCloseMove=false
    }

    function startComand1(){
        root.startComandOpen()
    }

    function startedClose(){
        closedMove=true
        startComand2()
    }

    function started2(){
        root.startedClose()
    }

    function startComandClose(){
        closedCmd=true
        manualOpenMove=false
        manualCloseMove=false
    }

    function startComand2(){
        root.startComandClose()
    }

    function stopComand(){
        openCmd=false
        closedCmd=false
        manualOpenMove=false
        manualCloseMove=false
    }
    function stoped(){
        openMove=false
        closedMove=false
        manualOpenMove=false
        manualCloseMove=false
        stopComand()
    }

    function manualOpen(){
        manualOpenMove=true
    }

    function manualWork1(){
        root.manualClose()
    }

    function manualClose(){
        manualCloseMove=true
    }

    function manualWork2(){
        root.manualClose()
    }
    function openSettings(){
        if(prop.visible==false){
            var absolutePos = Fap.getAbsolutePosition(canvas2);
            prop.setX(absolutePos.x)
            prop.setY(absolutePos.y)
            prop.show()
        }
        prop.requestActivate()
    }
    onOpenCmdChanged: {canvas1.requestPaint(); canvas2.requestPaint(); prop.st1=openCmd}
    onOpenMoveChanged: {canvas1.requestPaint(); canvas2.requestPaint();}
    onOpenedChanged: {canvas1.requestPaint(); canvas2.requestPaint(); prop.inSt1=opened}
    onClosedCmdChanged: {canvas1.requestPaint(); canvas2.requestPaint(); prop.st2=closedCmd}
    onClosedMoveChanged: {canvas1.requestPaint(); canvas2.requestPaint();}
    onClosedChanged: {canvas1.requestPaint(); canvas2.requestPaint(); prop.inSt2=closed}
    onAlarmChanged: {canvas1.requestPaint(); canvas2.requestPaint();}
    onQuitAlarmChanged: {canvas1.requestPaint(); canvas2.requestPaint();}
    onBlockedChanged: {canvas1.requestPaint(); canvas2.requestPaint();}
    onAvailableChanged: {canvas1.requestPaint(); canvas2.requestPaint();}
    onLinkedChanged: {canvas1.requestPaint(); canvas2.requestPaint();}
    onManualOpenMoveChanged: {canvas1.requestPaint(); canvas2.requestPaint();}
    onManualCloseMoveChanged: {canvas1.requestPaint(); canvas2.requestPaint();}

    Item {
        property int offset: 1
        id: gate
        width: parent.width/2 - offset*2
        height: parent.height - offset*2
        x: offset
        y: offset
        Canvas{
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    if((mouse.button & Qt.RightButton)&&(mouse.modifiers & Qt.ControlModifier)){
                    }else if((mouse.button & Qt.LeftButton)&&(mouse.modifiers & Qt.ControlModifier)){
                        root.addToCurrentRoteIn1pos()
                        root.addToCurrentRoteInOpened()
                    }else if(mouse.button & Qt.RightButton) {
                        if(prop.visible==false){
                            var absolutePos = Fap.getAbsolutePosition(canvas1);
                            prop.setX(absolutePos.x+mouseX)
                            prop.setY(absolutePos.y+mouseY)
                            prop.show()
                        }
                        prop.requestActivate()
                    }else{
                        if (openCmd||closedCmd){
                            root.stop()
                        }else{
                            root.start1()
                            root.open()
                        }
                    }
                }
                onPressAndHold: {
                    root.addToCurrentRoteIn1pos()
                    root.addToCurrentRoteInOpened()
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
            id: canvas1
            width:  parent.width/2+canvasOffset*2
            height: parent.height+canvasOffset*2
            x: -canvasOffset
            y: -canvasOffset
            visible: parent.visible
            onPaint: {
                Fap.changeColorG(root);
                var context = getContext("2d");
                context.strokeStyle = borderColor1;
                context.fillStyle = fillColor1;
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
            Text {
                id: op
                text: "o"
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                maximumLineCount: 1
                font: cl.font
            }
        }
        Canvas{
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    if((mouse.button & Qt.RightButton)&&(mouse.modifiers & Qt.ControlModifier)){

                    }else if((mouse.button & Qt.LeftButton)&&(mouse.modifiers & Qt.ControlModifier)){
                        root.addToCurrentRoteIn2pos()
                        root.addToCurrentRoteInClosed()
                    }else if(mouse.button & Qt.RightButton) {
                        if(prop.visible==false){
                            var absolutePos = Fap.getAbsolutePosition(canvas2);
                            prop.setX(absolutePos.x+mouseX)
                            prop.setY(absolutePos.y+mouseY)
                            prop.show()
                        }
                        prop.requestActivate()
                    }else{
                        if (openCmd||closedCmd){
                            root.stop()
                        }else{
                            root.start2()
                        }
                    }
                }
                onPressAndHold: {
                    if(prop.visible==false){
                        var absolutePos = Fap.getAbsolutePosition(canvas);
                        prop.setX(absolutePos.x+mouseX)
                        prop.setY(absolutePos.y+mouseY)
                        prop.show()
                    }
                }
            }
            id: canvas2
            width: parent.width/2+canvasOffset*2
            height: parent.height+canvasOffset*2
            x: parent.width/2-canvasOffset
            y: -canvasOffset
            onPaint: {
                Fap.changeColorG(root)
                var context = getContext("2d");
                context.clearRect(0,0,width,height);
                context.clearRect(0,0,width,height);
                context.strokeStyle = borderColor2;
                context.fillStyle = fillColor2;
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
            Text {
                id: cl
                text: "x"
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                maximumLineCount: 1
                font.pointSize: 12
                minimumPixelSize: 12
            }
        }
    }
    Text {
        visible: false
        id: gateText
        x: parent.width/2
        y: 0
        width: parent.width/2
        text: name
        enabled: true
        smooth: false
        antialiasing: true
        font.pixelSize: 10
        horizontalAlignment: Text.AlignLeft
        wrapMode: Text.Wrap
    }
    Component.onCompleted: {
        if (textPosition>0){
            gateText.visible = true
            if(textPosition==1){
                gateText.x = 0
                gate.x = gate.parent.width/2+gate.offset*3
                gateText.horizontalAlignment = Text.AlignHLeft
            }else{
                gateText.x = gateText.parent.width/2
                gate.x= gate.offset
                gateText.horizontalAlignment = Text.AlignHRight
            }
        }else{
            gate.width =  gate.parent.width
        }
    }
    PropertyWindow2 {
        id: prop
        objectName: "prop"
        winTitle: name
        onStart1: {
            root.start1()
            root.open()
        }
        onStart2: {
            root.start2()
            root.open()
        }
        onStop: root.stop()
        st1: root.openCmd
        st2: root.closedCmd
        inSt1: root.opened
        inSt2: root.closed
        //onResetAlarm: root.resetAlarm()
        buttonsType: 2
    }
}

///////////////////////////////////////////////////////////////////
///////////////////////////DESIGNER////////////////////////////////
///////////////////////////////////////////////////////////////////
//import QtQuick 2.12

//Item {
//    id: root
//    width: 83
//    height: 13
//    visible: true
//    property int borderWidth: 2
//    property int textPosition: 2
//    property string name: "unkown gate"

//    property int canvasOffset: 2

//    property color fillColor1: "green"
//    property color borderColor1: "black"
//    property color fillColor2: "green"
//    property color borderColor2: "black"

//    Item {
//        property int offset: 1
//        id: gate
//        width: parent.width/2 - offset*2
//        height: parent.height - offset*2
//        x: offset
//        y: offset
//        Canvas{
//            id: canvas1
//            width:  parent.width/2+canvasOffset*2
//            height: parent.height+canvasOffset*2
//            x: -canvasOffset
//            y: -canvasOffset
//            visible: parent.visible
//            onPaint: {
//                var context = getContext("2d");
//                context.strokeStyle = borderColor1;
//                context.fillStyle = fillColor1;
//                context.lineWidth = borderWidth*2;
//                context.beginPath();
//                context.moveTo(canvasOffset, canvasOffset);
//                context.lineTo(width-canvasOffset, canvasOffset);
//                context.lineTo(width-canvasOffset, height-canvasOffset);
//                context.lineTo(canvasOffset, height-canvasOffset);
//                context.lineTo(canvasOffset, canvasOffset)
//                context.closePath;
//                context.stroke();
//                context.fill();
//            }
//            Text {
//                text: "o"
//                anchors.fill: parent
//                horizontalAlignment: Text.AlignHCenter
//                verticalAlignment: Text.AlignVCenter
//                font.pixelSize: 11
//                font.bold: false
//            }
//        }
//        Canvas{
//            id: canvas2
//            width: parent.width/2+canvasOffset*2
//            height: parent.height+canvasOffset*2
//            x: parent.width/2-canvasOffset
//            y: -canvasOffset
//            onPaint: {
//                var context = getContext("2d");
//                context.strokeStyle = borderColor2;
//                context.fillStyle = fillColor2;
//                context.lineWidth = borderWidth*2;
//                context.beginPath();
//                context.moveTo(canvasOffset, canvasOffset);
//                context.lineTo(width-canvasOffset, canvasOffset);
//                context.lineTo(width-canvasOffset, height-canvasOffset);
//                context.lineTo(canvasOffset, height-canvasOffset);
//                context.lineTo(canvasOffset, canvasOffset)
//                context.closePath;
//                context.stroke();
//                context.fill();
//            }
//            Text {
//                text: "x"
//                anchors.fill: parent
//                horizontalAlignment: Text.AlignHCenter
//                verticalAlignment: Text.AlignVCenter
//                font.pixelSize: 11
//                font.bold: false
//            }
//        }
//    }
//    Text {
//        visible: false
//        id: gateText
//        x: parent.width/2
//        y: 0
//        width: parent.width/2
//        text: name
//        enabled: true
//        smooth: false
//        antialiasing: true
//        font.pixelSize: 10
//        horizontalAlignment: Text.AlignLeft
//        wrapMode: Text.Wrap
//    }
//    Component.onCompleted: {
//        if (textPosition>0){
//            gateText.visible = true
//            if(textPosition==1){
//                gateText.x = 0
//                gate.x = gate.parent.width/2+gate.offset*3
//                gateText.horizontalAlignment = Text.AlignHLeft
//            }else{
//                gateText.x = gateText.parent.width/2
//                gate.x= gate.offset
//                gateText.horizontalAlignment = Text.AlignHRight
//            }
//        }else{
//            gate.width =  gate.parent.width
//        }
//    }
//}

///////////////////////////////////////////////////////////////////
///////////////////////////DESIGNER////////////////////////////////
///////////////////////////////////////////////////////////////////
