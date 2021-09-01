import QtQuick 2.12

Item {
    width: 100
    height: 100
    visible: true
    property int borderWidth: 2
    property int canvasOffset: 10
    property string name: "unknown silo"
    property bool heavy: true
    property bool promise: true
    Canvas {
        id: silo
        width: parent.width+canvasOffset*2
        height: parent.height+canvasOffset*2
        x: -canvasOffset
        y: -canvasOffset
        visible: parent.visible
        onPaint: {
            var context = getContext("2d");
            context.clearRect(0,0,width,height);
            context.strokeStyle = "black";
            context.fillStyle = "gray"
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
    Text{
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: name
        visible: parent.visible
    }
    Text{
        anchors.fill: parent
        anchors.topMargin: 30
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "Скоро тут будет окно управления сушилкой"
        //visible: root.promise
    }
}
