import QtQuick 2.12

Item {
    width: 100
    height: 100
    property bool upload: false
    property int borderWidth: 2
    property string name: "unknown dryier"
    property real wk: 0.0024 * width
    property real hk: 0.0018 * height
    Canvas {
        id: canvas
        anchors.fill: parent
        visible: parent.visible
        onPaint: {
            var context = getContext("2d");
            context.clearRect(0,0,width,height);
            context.strokeStyle = "black";
            context.fillStyle = "gray"
            context.lineWidth = borderWidth;
            context.clearRect(0,0,width,height);
            context.moveTo(400 *wk, 550 *hk);
            context.lineTo(10  *wk, 550 *hk);
            context.lineTo(10  *wk, 330 *hk);
            context.lineTo(140 *wk, 250 *hk);
            context.lineTo(140 *wk, 120 *hk);
            context.lineTo(190 *wk, 20  *hk);
            context.lineTo(220 *wk, 20  *hk);
            context.lineTo(270 *wk, 120 *hk);
            context.lineTo(270 *wk, 185 *hk);
            context.lineTo(400 *wk, 250 *hk);
            context.lineTo(400 *wk, 550 *hk);
            context.closePath;
            context.stroke();
            context.fill(); //контур

            context.moveTo(270 *wk, 550 *hk);
            context.lineTo(270 *wk, 120 *hk);
            context.stroke(); //правая вертикальная линия

            context.moveTo(140 *wk, 550 *hk);
            context.lineTo(140 *wk, 120 *hk);
            context.stroke(); //левая вертикальная линия

            context.moveTo(140 *wk, 120 *hk);
            context.lineTo(270 *wk, 120 *hk);
            context.stroke(); //1 горизонтальная линия

            context.moveTo(140 *wk, 185 *hk);
            context.lineTo(270 *wk, 185 *hk);
            context.stroke(); //2 горизонтальная линия

            context.moveTo(140 *wk, 250 *hk);
            context.lineTo(270 *wk, 250 *hk);
            context.stroke(); //3 горизонтальная линия

            context.moveTo(140 *wk, 315 *hk);
            context.lineTo(400 *wk, 315 *hk);
            context.stroke(); //4 горизонтальная линия

            context.moveTo(140 *wk, 370 *hk);
            context.lineTo(270 *wk, 370 *hk);
            context.stroke(); //5 горизонтальная линия

            context.moveTo(140 *wk, 435 *hk);
            context.lineTo(270 *wk, 435 *hk);
            context.stroke(); //6 горизонтальная линия

            context.moveTo(140 *wk, 500 *hk);
            context.lineTo(270 *wk, 500 *hk);
            context.stroke(); //7 горизонтальная линия
        }

    }
    Canvas{
        id: canvasTr
        anchors.fill: parent
        visible: parent.visible
        onPaint: {
            var context = getContext("2d");
            context.clearRect(0,0,width,height);
            context.strokeStyle = "black";
            context.lineWidth = borderWidth * 2;
            context.moveTo(140 *wk +2, 505 *hk  );
            context.lineTo(270 *wk -3, 505 *hk  );
            context.lineTo(205 *wk   , 550 *hk -2);
            context.lineTo(140 *wk +2, 505 *hk  );
            context.closePath;
            context.stroke();
            if (upload){
                context.fillStyle = "green"

            }else{
                context.fillStyle = "gray"
            }
            context.fill(); //Треугольник
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                upload = !upload
                canvas.requestPaint()
                canvasTr.requestPaint()
            }
        }
    }

    Text{
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: name
    }
}
