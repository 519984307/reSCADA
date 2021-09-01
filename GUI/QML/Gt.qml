import QtQuick 2.0

Item {
    id: itemMain
    width: 160
    height: 80
    Canvas{
        id: canvasOP
        width:  parent.width/2
        height: parent.height
        x: 0
        y: 0
        property int offset: Math.min(parent.width, parent.height)
        onPaint: {
            var context = getContext("2d");
            context.strokeStyle = "black";
            context.fillStyle = "white";
            context.lineWidth = 1;
            context.beginPath();
            context.moveTo(0, 0);
            context.lineTo(width, 0);
            context.lineTo(width, height);
            context.lineTo(0, height);
            context.lineTo(0, 0)
            context.fill();
            context.stroke();
            context.beginPath();
            context.arc((width)/2, (height)/2, offset, 0, 2 * Math.PI);
            //context.fill();
            context.stroke();
        }
    }
    Canvas{
        id: canvasCL
        width:  parent.width/2
        height: parent.height
        anchors.right: parent.right
        anchors.rightMargin: 0
        x: 0
        y: 0
        onPaint: {
            var context = getContext("2d");
            context.strokeStyle = "black";
            context.fillStyle = "white";
            context.lineWidth = 1;
            context.beginPath();
            context.moveTo(0, 0);
            context.lineTo(width, 0);
            context.lineTo(width, height);
            context.lineTo(0, height);
            context.lineTo(0, 0)
            context.fill();
            context.lineTo(width, height)
            context.moveTo(width, 0);
            context.lineTo(0, height)
            context.stroke();
            context.beginPath();

            //context.fill();
            context.stroke();
        }
    }
}


