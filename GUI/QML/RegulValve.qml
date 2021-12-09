import QtQuick 2.15
import QtGraphicalEffects 1.15

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
    property bool nameOnTop: false

    //    onNameOnLeftChanged: nameText.horPos = nameOnLeft ? 1 : -1
    //    onNameOnTopChanged: nameText.vertPos = nameOnLeft ? -1 : 1

    width: 40
    height: 40
    onLevelChanged: {
        cnvs.requestPaint()
    }

    Text {
        id: nameText
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: qsTr("РК")

        font.pixelSize: 20
        font.bold: true
        wrapMode: Text.NoWrap
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: (nameOnLeft ? -1 : 1) * parent.width/2
        anchors.horizontalCenterOffset: (nameOnTop ? 1 : -1) * parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        fontSizeMode: Text.HorizontalFit
    }
    Rectangle{
        id: rectBody
        border.width: borderWidth
        border.color: borderColor
        anchors.fill: parent
        radius: width * 0.2
        Rectangle {
            id: rectColor
            radius: parent.radius - parent.border.width
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.rightMargin: parent.border.width
            anchors.leftMargin: parent.border.width
            anchors.topMargin: parent.border.width
            anchors.bottomMargin: parent.border.width
            color: borderColor
            Rectangle {
                id: mSrc
                anchors.fill: parent
                radius: rectColor.radius - rectBody.border.width
                color: borderColor
            }
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: mSrc
            }
            Canvas {
                id: cnvs
                anchors.fill: parent
                onPaint: {
                    var ctx = getContext("2d");
                    ctx.reset();

                    var centreX = width / 2;
                    var centreY = height / 2;

                    ctx.beginPath();
                    ctx.fillStyle = contItem.backgroundColor;
                    ctx.moveTo(centreX, centreY);
                    ctx.arc(centreX, centreY, width, Math.PI/2, contItem.level * 2 * Math.PI + Math.PI/2, false);
                    ctx.lineTo(centreX, centreY);
                    ctx.fill();
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}
}
##^##*/
