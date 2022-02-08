import QtQuick 2.15
import QtQuick.Controls 2.15
import "fap.js" as Fap

UnitItem {
    id: root
    width: 60
    height: 15
    property alias mouseArea: mAr
    property color colorShortName: "green"
    property alias shortNameText: nameLable.text
    property alias valueText: valueLable.text
    property alias tooltipText: tTip.text
    property int mantissa: 1
    borderWidth: 1
    backgroundColor: "transparent"
    onAlarmDescrChanged: {
        tooltipText = name
        if( alarmDescr != ""){
            tooltipText += "\n" + alarmDescr
        }
    }

    function setValue(value) {
        //уст значение
        if (Fap.isString(value))
            value = Number(value)
        value = value.toFixed(mantissa)
        valueLable.text = value
    }

    Rectangle {
        id: rectShortName
        anchors.left: parent.left
        height: parent.height
        width: parent.height
        border.width: borderWidth
        color: colorShortName
        border.color: borderCurrentColor
        Text {
            id: nameLable
            text: qsTr("T")
            anchors.fill: parent
            font.pixelSize: parent.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
        }
    }

    Rectangle {
        id: rectValue
        anchors.left: rectShortName.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        border.color: borderCurrentColor
        border.width: borderWidth
        color: backgroundCurrentColor
        Text {
            id: valueLable
            text: qsTr("999.9")
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: height * 0.7
            font.bold: true
            font.family: "DSEG7 Classic"
        }
        MouseArea {
            id: mAr
            anchors.fill: parent
            acceptedButtons: Qt.RightButton | Qt.LeftButton
            hoverEnabled: true
            onClicked: {
                alarmNotify = false
                notify = false
            }
            onEntered: {
                tTip.visible = true
            }
            onExited: {
                tTip.visible = false
            }
        }
        ToolTip {
            id: tTip
            delay: 500
            timeout: 5000
            visible: false
            text: name
        }
    }
}




/*##^##
Designer {
    D{i:0;formeditorZoom:6}
}
##^##*/
