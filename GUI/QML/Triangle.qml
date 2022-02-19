import QtQuick 2.15
import "fap.js" as Fap

Item {
    property bool active: false
    property int borderWidth: 1
    property color activeColor: Fap.ready
    property color nActiveColor: Fap.defaultColor
    property color borderColor: Fap.border
    width: 20
    height: 20
    visible: true
    antialiasing: false
    onActiveChanged: pipe.requestPaint()

    Canvas {
        id: pipe
        width: parent.width
        height: parent.height
        x: 0
        y: 0
        visible: parent.visible
        onPaint: {
            var ctx = getContext("2d")
            ctx.strokeStyle = borderColor
            ctx.lineWidth = borderWidth
            ctx.lineCap = "butt"
            ctx.lineJoin = "round"
            ctx.beginPath()
            ctx.moveTo(borderWidth, borderWidth / 2)
            ctx.lineTo(width - borderWidth, borderWidth / 2)
            ctx.lineTo(width / 2, height - borderWidth)
            ctx.closePath()
            ctx.stroke()
            ctx.fillStyle = active ? activeColor : nActiveColor
            ctx.fill()
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.5;height:437.5;width:600}
}
##^##*/

