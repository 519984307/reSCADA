import QtQuick 2.12
//import QtQuick.Controls 1.3
import QtQuick.Controls 2.12
import "fap.js" as Fap

UnitItem {
    id: root
    width: 180
    height: 40
    //backgroundColor: "white"
    property color colorShortName: "green"
    borderColor: "black"
    borderWidth: 1
    //property int fontSize: 12
    property alias nameText: nameLable.text
    property alias valueText: valueLable.text
    tooltipText:"Сигнал"
    property int upLimit: 100
    property int downLimit: 0
    //property alias regexp: valueLable.validator
    property int mantissa: 1
    property real extAlarmLevel: 0
    property real realValue: 0


    function setValue( value ){//уст значение
        value = value.toFixed( mantissa )
        realValue = Number( value )
        valueLable.text = value
    }

    Rectangle{
        id: rectShortName
        anchors.left: parent.left
        anchors.leftMargin: 0
        height: parent.height
        width: parent.height
        border.width: borderWidth
        color: colorShortName
        border.color: borderCurrentColor
        Text{
            id: nameLable
            text: qsTr("P")
            anchors.fill: parent
            font.pixelSize: parent.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
        }
    }

    Rectangle{
        id: rectValue
        color: backgroundCurrentColor
        border.color: borderCurrentColor
        border.width: borderWidth
        anchors.left: rectShortName.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        Text{
            id: valueLable
            text: "999.9"
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height * 0.8
            font.bold: true
            font.family: "DSEG7 Classic"
            //readOnly: true
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}
}
##^##*/
