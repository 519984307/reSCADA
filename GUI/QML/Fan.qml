import QtQuick 2.12
import "fap.js" as Fap

UnitItem {
    id: fan
    width: 30
    height: 30
    visible: true
    property alias fanName: fanName
    property bool mirror: false
    property int rotatePict: 0
    property alias textX: fanName.x
    property alias textY: fanName.y

    property bool st: false
    property bool std: false
    property bool manual: false
    property bool cleaning: false

    signal start
    signal stop
    signal addToCurrentRoteStarted
    signal addToCurrentRoteStoped

    function started() {
        startComand()
        std = true
        manual = false
        cleaning = false
    }
    function stoped() {
        stopComand()
        std = false
        manual = false
        cleaning = false
    }
    function startComand() {
        st = true
        manual = false
        cleaning = false
    }
    function stopComand() {
        st = false
        manual = false
        cleaning = false
    }
    function manualWork() {
        manual = true
    }

    function cleaningWork() {
        cleaning = true
    }

    onStChanged: canvas.requestPaint()
    onStdChanged: canvas.requestPaint()
    onAlarmChanged: canvas.requestPaint()
    onBlockedChanged: canvas.requestPaint()
    onLinkedChanged: canvas.requestPaint()
    onManualChanged: canvas.requestPaint()

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
            context.lineWidth = borderWidth * 2
            context.beginPath()
            context.moveTo(width * 0.85, borderWidth)
            context.lineTo(width * 0.85, height - width / 2)
            context.lineTo(width - borderWidth, height - width / 2)
            context.lineTo(width - borderWidth, borderWidth)
            context.closePath()
            context.stroke()
            context.fill()
            //Круг
            context.beginPath()
            context.arc(width / 2, height - width / 2, Math.max(
                            width, height) / 2 - borderWidth, 0, 2 * Math.PI)
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
}

/*##^##
Designer {
    D{i:0;height:55;width:56}
}
##^##*/

