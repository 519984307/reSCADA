import QtQuick 2.12
import "fap.js" as Fap

Item {
    id: root
    width: 100
    height: 100
    visible: true
    property int borderWidth: 2
    property int canvasOffset: 10
    property string name: "unknown silo"
    property alias fontSize: textField.font.pointSize
    property bool heavy: true
    property alias alarm: prop.alarm
    property alias available: prop.available
    property alias linked: prop.linked
    property int indicatorOffset: 0

    signal addToCurrentRoteFull()
    signal addToCurrentRoteEmpty()

    onAlarmChanged: canvas.requestPaint()

    function activateHL(){
        indicator.activateHL()
    }
    function deactivateHL(){
        indicator.deactivateHL()
    }
    function activateML(){
        indicator.activateML()
    }
    function deactivateML(){
        indicator.deactivateML()
    }
    function activateLL(){
        indicator.activateLL()
    }
    function deactivateLL(){
        indicator.deactivateLL()
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
    Canvas {
        MouseArea{
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if(mouse.modifiers & Qt.ShiftModifier){
                    root.addToCurrentRoteEmpty()
                }else if(mouse.modifiers & Qt.ControlModifier){
                    root.addToCurrentRoteFull()
                }else if(mouse.button & Qt.RightButton) {
                    if(prop.visible==false){
                        var absolutePos = Fap.getAbsolutePosition(canvas);
                        prop.setX(absolutePos.x+mouseX)
                        prop.setY(absolutePos.y+mouseY)
                        prop.show()
                    }
                    prop.requestActivate()
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
            var context = getContext("2d");
            context.strokeStyle = root.alarm?Fap.alarm:Fap.notAlarm;
            context.fillStyle = linked?(available?"gray":Fap.notAvailable):Fap.defaultColor
            context.clearRect(0,0,width,height);
//            context.strokeStyle = "black"
//            context.fillStyle = "grey"

            context.lineWidth = borderWidth*2;
            context.beginPath();
            if (heavy){
                context.moveTo(canvasOffset, height-canvasOffset);
                context.lineTo(width-canvasOffset, height-canvasOffset);
            }
            else{
                context.moveTo(canvasOffset, height-height/3-canvasOffset);
                context.lineTo(width/3+canvasOffset, height-canvasOffset);
                context.lineTo(width-width/3-canvasOffset, height-canvasOffset);
                context.lineTo(width-canvasOffset, height-height/3-canvasOffset);
            }
            context.lineTo(width-canvasOffset, canvasOffset+height/5);
            context.lineTo(width-width/3-canvasOffset, canvasOffset);
            context.lineTo(canvasOffset+width/3, canvasOffset);
            context.lineTo(canvasOffset, canvasOffset+height/5);
            if (heavy){
                context.lineTo(canvasOffset, height-canvasOffset);
            }else{
                context.lineTo(canvasOffset, height-height/3-canvasOffset);
            }
            context.closePath;
            context.stroke();
            context.fill();
        }

    }    
    TextEdit {
        id: textField
        text: prop.exName==""?name:prop.exName
        anchors.right: parent.right
        anchors.left: parent.left

        anchors.top: parent.top
        anchors.topMargin: 28
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 32

        horizontalAlignment: TextEdit.AlignHCenter
        verticalAlignment: TextEdit.AlignVCenter
        onEditingFinished: {
            prop.changedExName(text)
        }
        Keys.onPressed:{
            if ((event.key===16777220 || event.key===16777221) && (event.modifiers & Qt.ControlModifier)){ //Enter
                focus = false;
            }
        }
    }
    Indicator2{
        id: indicator
        anchors.leftMargin: 2
        anchors.left: parent.left
        anchors.topMargin: root.height * 0.23
        anchors.top: parent.top
        anchors.bottomMargin: root.indicatorOffset===0?(root.heavy?32:root.height * 0.39):root.indicatorOffset
        anchors.bottom: parent.bottom
        multicolor: true
    }

    PropertyWindow2 {
        id: prop
        objectName: "prop"
        winTitle: name
        onExNameChanged: {textField.text = prop.exName}
        buttonsType: 4
    }
}
