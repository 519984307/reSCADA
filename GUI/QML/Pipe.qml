import QtQuick 2.12
import "fap.js" as Fap

Item {
    property bool active: false
    property int borderWidth: 2
    property int angle: 90
    property int pipeThin: 6
    property color acolor: Fap.run
    property color nacolor: Fap.ready
    width: 60
    height: 100
    visible: true
    antialiasing: false
    onActiveChanged: pipe.requestPaint()
    onAngleChanged: pipe.requestPaint()

    Canvas {
//        MouseArea {
//            anchors.fill: parent
//            onClicked: {
//                active=!active;
//            }
//        }
        id: pipe
        width: parent.width
        height: parent.height
        x: 0
        y: 0
        visible: parent.visible
        onPaint: {
            var context = getContext("2d");
            context.clearRect(0,0,width,height);
            context.lineJoin = "round";
            context.strokeStyle = Fap.notAlarm;
            context.lineWidth = pipeThin+borderWidth*2;
            context.beginPath();
            context.moveTo(width/2, 0);
            context.lineTo(width/2, height/2);
            context.lineTo(width/2+width*height*Math.cos(angle*Math.PI/180), height/2+width*height*Math.sin(angle*Math.PI/180));
            context.stroke();

            if (active){
                context.strokeStyle = acolor
            }else{
                context.strokeStyle = nacolor
            }
            context.lineWidth = pipeThin;
            context.beginPath();
            context.moveTo(width/2, 0);
            context.lineTo(width/2, height/2);
            context.lineTo(width/2+width*height*Math.cos(angle*Math.PI/180), height/2+width*height*Math.sin(angle*Math.PI/180));
            context.stroke();
        }
    }
}
