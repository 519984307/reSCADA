import QtQuick 2.12
//import QtQuick.Controls 1.3
import QtQuick.Controls 2.12
import "fap.js" as Fap

Item {
    id: root
    width: 180
    height: 40
    property alias mouseArea: mAr
    property alias backgroundColor: rectValue.color
    property color colorShortName: "green"
    property alias borderColor: rectValue.border.color
    property alias borderWidth: rectValue.border.width
    property alias shortNameText: nameLable.text
    property alias valueText: valueLable.text
    property alias tooltipText: tTip.text
    property int mantissa: 1

    function setValue( value ){//уст значение
        if( Fap.isString(value) ) value = Number( value )
        value = value.toFixed( mantissa )
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
        border.color: borderColor
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
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height * 0.8
            font.bold: true
            font.family: "DSEG7 Classic"
            //readOnly: true
        }
        MouseArea{
            id:mAr
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                tTip.visible = true
                setValue(556.36)
            }
            onExited: {
                tTip.visible = false
            }
        }
        ToolTip{
            id: tTip
            delay: 2000
            timeout: 4000
            visible: false
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}
}
##^##*/
