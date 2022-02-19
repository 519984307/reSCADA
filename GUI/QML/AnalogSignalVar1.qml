import QtQuick 2.15
import QtQuick.Controls 2.15
import "fap.js" as Fap

AnalogSignal {
    id: asi
    width: 71
    height: 25
    property alias rectShortName: rectShortName
    property color colorShortName: "green"
    property alias shortNameText: nameLable.text
    property bool shVisible: true
    property bool shTop: false
    property bool shBottom: false
    property bool shLeft: true
    property bool shRight: false
    property int shHeight: height
    property int shWidth: height

    Rectangle {
        id: rectShortName
        height: parent.shHeight
        width: parent.shWidth
        visible: shVisible
        border.width: parent.borderWidth
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset:
            ((shTop||shBottom)?(shTop?-1:1):0)*(height - border.width)
        anchors.horizontalCenterOffset:
            (shLeft||shRight)?((shLeft?-1:1)*(parent.width/2 + ((shTop||shBottom)?-width/2:(width/2 - border.width)))):0
        anchors.horizontalCenter: parent.horizontalCenter
        color: colorShortName
        border.color: borderCurrentColor
        Text {
            id: nameLable
            text: ""
            anchors.fill: parent
            font.pixelSize: parent.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:4}
}
##^##*/
