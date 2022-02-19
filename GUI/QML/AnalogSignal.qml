import QtQuick 2.15
import QtQuick.Controls 2.15
import "fap.js" as Fap

UnitItem {
    id: root
    width: 90
    height: 30
    property alias rectValue: rectValue
    property alias mouseArea: mAr
    property alias valueText: valueLable.text
    property alias tooltipText: tTip.text
    property int mantissa: 1
    property string postfix: ""
    backgroundColor: "white"
    borderColor: "black"
    borderWidth: 1
    borderWidthNotify: 2
    onAlarmDescrChanged: {
        tooltipText = name
        if( alarmDescr != ""){
            tooltipText += "\n" + alarmDescr
        }
    }

    function setValue(value) {
        //уст значение
        //if (Fap.isString(value))
        value = Number(value)
        value = value.toFixed(mantissa)
        valueLable.text = "<b>"+value+"</b>" +postfix
    }

    Rectangle {
        id: rectValue
        color: parent.backgroundCurrentColor
        border.color: parent.borderCurrentColor
        anchors.fill: parent
        border.width: parent.borderCurrentWidth
        Text {
            id: valueLable
            text: "999.9"
            anchors.fill: parent
            font.pixelSize: 400
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.rightMargin: 1
            anchors.leftMargin: 1
            minimumPointSize: 3
            fontSizeMode: Text.Fit
            //font.pixelSize: height * 0.95
            //font.bold: true
            //font.family: "DSEG7 Classic"
        }

    }
    MouseArea {
        id: mAr
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        hoverEnabled: true
        onClicked: {
            alarmNotify = false
            notify = false
//            if(!linked)linked = true
//            else if(!connected)
//                connected = true
//            else if(!alarm)
//                setAlarm("sd")
//            else if(!alarmNotify)
//                setQuitAlarm("ad")
//            else{
//                alarmReseted()
//                linked =false
//                connected = false
//            }
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




/*##^##
Designer {
    D{i:0;formeditorZoom:3}
}
##^##*/
