import QtQuick 2.15
import "fap.js" as Fap

Item {
    width: 300
    height: 300
    visible: true
    antialiasing: true

    property int borderWidth: 2
    property int pipeThin: 10
    property bool active: false
    property color activeColor: Fap.ready
    property color nActiveColor: Fap.defaultColor
    property color borderColor: Fap.border
    property real startAmgle: 0
    property real endAmgle: 360
    property int xCenter: width / 2
    property int yCenter: height / 2
    property int radius: Math.min(width, height) / 2

    onNActiveColorChanged: pipe.requestPaint()
    onBorderColorChanged: pipe.requestPaint()
    onPipeThinChanged: pipe.requestPaint()
    onBorderWidthChanged: pipe.requestPaint()
    onWidthChanged: pipe.requestPaint()
    onHeightChanged: pipe.requestPaint()
    onStartAmgleChanged: pipe.requestPaint()
    onEndAmgleChanged: pipe.requestPaint()

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
            ctx.arc(xCenter, yCenter, radius - pipeThin / 2,
                    startAmgle * Math.PI/180, endAmgle * Math.PI/180)
            ctx.stroke()
            ctx.beginPath()
            ctx.lineJoin = "miter"
            ctx.strokeStyle = active ? activeColor : nActiveColor
            ctx.lineWidth = pipeThin - 2 * borderWidth
            ctx.arc(xCenter, yCenter, radius - pipeThin / 2,
                    startAmgle * Math.PI/180, endAmgle * Math.PI/180)
            ctx.stroke()
        }
    }
}
