import QtQuick 2.15
import "fap.js" as Fap

StartStopUnit {
    id: fan
    width: 30
    height: 30
    visible: true
    property alias fanName: fanName
    property bool mirror: false
    property int rotatePict: 0
    property alias textX: fanName.x
    property alias textY: fanName.y

//    onStChanged: canvas.requestPaint()
//    onStdChanged: canvas.requestPaint()
//    onManualChanged: canvas.requestPaint()
    onBorderCurrentWidthChanged: canvas.requestPaint()
    onBorderCurrentColorChanged: canvas.requestPaint()
    onBackgroundCurrentColorChanged:canvas.requestPaint()

    Canvas {
        id: canvas
        anchors.fill: parent
        rotation: rotatePict
        visible: parent.visible
        transform: Scale {
            origin.x: width / 2
            xScale: mirror ? -1 : 1
            yScale: 1
        }
        onPaint: {
            //Пряоугольник
            var context = getContext("2d")
            context.clearRect(0, 0, width, height)
            context.strokeStyle = borderCurrentColor
            context.fillStyle = backgroundCurrentColor
            context.lineWidth = borderCurrentWidth * 2
            context.beginPath()
            context.moveTo(width * 0.85, borderCurrentWidth)
            context.lineTo(width * 0.85, height - width / 2)
            context.lineTo(width - borderCurrentWidth, height - width / 2)
            context.lineTo(width - borderCurrentWidth, borderCurrentWidth)
            context.closePath()
            context.stroke()
            context.fill()
            //Круг
            context.beginPath()
            context.arc(width / 2, height - width / 2, Math.max(
                            width, height) / 2 - borderCurrentWidth, 0, 2 * Math.PI)
            context.stroke()
            context.fill()
        }
    }
    Text {
        id: fanName
        text: name
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 7
        visible: parent.visible
        wrapMode: Text.Wrap
    }
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onDoubleClicked: {
            if (st)
                s_stop()
            else
                s_start()
        }
        onClicked: {
            alarmNotify = false;
            if (mouse.button & Qt.RightButton) {
                openSettings()
            }
            //            else if(mouse.modifiers & Qt.ShiftModifier){
            //                addToCurrentRoteStoped()
            //            }
            //            else if(mouse.modifiers & Qt.ControlModifier){
            //                addToCurrentRoteStarted()
            //            }
        }
    }
}

/*##^##
Designer {
    D{i:0;height:55;width:56}
}
##^##*/
