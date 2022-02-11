import QtQuick 2.12

Tank {
    id: tankUnit
    width: 70
    height: 350
    property alias unit: ut
    onS_enableMouseArea: ma.enabled = Enable
    MouseArea {
        id: ma
        //visible: false
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: {
            if (mouse.button & Qt.RightButton) {
                ut.openSettings()
            }
        }
    }

    UnitPropItem{
        id: ut
        objectName: "unit"
        function f1(){parent.mainGradientColor = backgroundCurrentColor}
        function f2(){parent.borderColor = borderCurrentColor}
        Component.onCompleted: {
            backgroundColor= parent.mainGradientColor
            borderColor = parent.borderColor
            onBackgroundCurrentColorChanged.connect(f1)
            onBorderCurrentColorChanged.connect(f2)
        }
    }
}

//signal alarmBottomLevelChanged(int value)

//++++++++ Test +++++++
//        mouseArea.onPressAndHold: {
//            linked = true
//            connected = true
//            allovAlarmBlinck = true
//            setQuitAlarm()
//            var cl = tank.mainGradientColor
//            cl = backgroundCurrentColor
//        }
//    // ------ Test ------
