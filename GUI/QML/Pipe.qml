import QtQuick 2.15
import "fap.js" as Fap

Item {
    width: 200
    height: 5
    property bool active: false
    property int borderWidth: 2
    property bool horOrVert: true
    property color activeColor: Fap.run
    property color nActivColor: Fap.defaultColor
    property color borderColor: Fap.border
    antialiasing: false
    onActiveChanged: pipe.requestPaint()
    onHorOrVertChanged: pipe.requestPaint()

    Canvas {
        id: pipe
        visible: parent.visible
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d")
            ctx.fillStyle = borderColor
            ctx.rect(0, 0, width, height)
            ctx.fill()
            ctx.lineJoin = "miter"
            ctx.strokeStyle = active ? activeColor : nActivColor
            ctx.lineWidth = (horOrVert ? height : width) - 2 * borderWidth
            ctx.beginPath()
            ctx.moveTo(horOrVert ? 0 : width / 2, horOrVert ? height / 2 : 0)
            ctx.lineTo(horOrVert ? width : width / 2,
                       horOrVert ? height / 2 : height)
            ctx.stroke()
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:4}
}
##^##*/

