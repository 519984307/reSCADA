import QtQuick 2.15

Item{
    id: contItem
    property real  level: 0.4
    property alias nameText: nameText
    property alias nameTextPixSize: nameText.font.pixelSize
    property bool  showLevel: true
    property int   borderWidth: 2
    property color borderColor: "black"
    property color backgroundColor: "steelblue"
    property bool nameOnLeft: true
    property bool nameOnTop: true

//    onNameOnLeftChanged: nameText.horPos = nameOnLeft ? 1 : -1
//    onNameOnTopChanged: nameText.vertPos = nameOnLeft ? -1 : 1

    width: 40
    height: 40

    Rectangle {
        id: rectBody
        height: contItem.height
        width: height
        border.width: contItem.borderWidth
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        border.color: contItem.borderColor
        color: contItem.backgroundColor
        radius: width/2
        Text {
            id: nameText
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: qsTr("РК1")

            font.pixelSize: 20
            font.bold: true
            wrapMode: Text.NoWrap
           anchors.verticalCenter: parent.verticalCenter
           anchors.verticalCenterOffset: (nameOnLeft ? -1 : 1) * parent.width/2
           anchors.horizontalCenterOffset: (nameOnTop ? 1 : -1) * parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            fontSizeMode: Text.HorizontalFit
        }
        Canvas {
            anchors.fill: rectBody
            onPaint: {
                var ctx = getContext("2d");
                ctx.reset();

                var centreX = width / 2;
                var centreY = height / 2;

                ctx.beginPath();
                ctx.fillStyle = contItem.borderColor;
                ctx.moveTo(centreX, centreY);
                ctx.arc(centreX, centreY, width / 2 - rectBody.border.width , 0, contItem.level * 2 * Math.PI, false);
                ctx.lineTo(centreX, centreY);
                ctx.fill();
            }
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}
}
##^##*/
