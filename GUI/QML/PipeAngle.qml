import QtQuick 2.15
import "fap.js" as Fap

Item {
    property bool active: false
    property int borderWidth: 2
    property int pipeThin: 10
    property color activeColor: Fap.run
    property color nActivColor: Fap.defaultColor
    property color borderColor: Fap.border
    width: 300
    height: 300
    visible: true
    antialiasing: true
    onActiveChanged: pipe.requestPaint()
    onHeightChanged: pipe.requestPaint()

    Canvas {
        id: pipe
        anchors.fill: parent
        visible: parent.visible
        onPaint: {
            var ctx = getContext("2d")
            ctx.beginPath()
            ctx.strokeStyle = borderColor
            ctx.lineJoin = "miter"
            ctx.lineWidth = pipeThin
            ctx.arc(0, 0, width - pipeThin / 2, 0, Math.PI)
            ctx.stroke()
            ctx.beginPath()
            ctx.lineJoin = "miter"
            ctx.strokeStyle = active ? activeColor : nActivColor
            ctx.lineWidth = pipeThin - 2 * borderWidth
            ctx.arc(0, 0, width - pipeThin / 2, 0, Math.PI)
            ctx.stroke()
        }
    }
}
