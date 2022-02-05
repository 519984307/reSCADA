import QtQuick 2.12

UnitItem {
    id: tankUnit
    width: 70
    height: 350

    property alias level: tank.level
    property alias alarmTopLevel: alarmTopLevel.valueReal
    property alias alarmBottomLevel: alarmBottomLevel.valueReal
    property alias tank: tank

    function setLevel( Level ) {
        level = Level
    }
    function setAlarmLevelTop(value) {
        alarmTopLevel.setValue(value)
    }
    //signal alarmTopLevelChanged(int value)

    function setAlarmLevelBottom(value) {
        alarmBottomLevel.setValue(value)
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
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: {
            if (mouse.button & Qt.RightButton) {
                unP.openSettings()
            }
        }
        //        onPressAndHold: {
        //            setLvl( level + 0.05 ) test
        //        }
    }
    Tank {
        id: tank
        objectName: tankUnit.objectName + ".tank"
        anchors.fill: parent
        mainGradientColor: backgroundCurrentColor
        borderWidth: borderWidth
        borderColor: borderCurrentColor
        nameText.text: name
    }
    MFUnit {
        id: alarmTopLevel
        width: 54
        objectName: tankUnit.objectName + ".alarmTopLevel"
        height: 20
        backgroundColor: "#f03e3e"
        tooltip: "Предельный верхний уровень"
        readOnly: false
        visible: true
        anchors.left: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 0
        anchors.topMargin: 0
        valueFontSize.bold: false
        disappear: true
        correctingButtons: true
        onValueRealChanged: tankUnit.alarmTopLevelChanged(valueReal)
        checkLimit: true
        downLimit: 20
    }
    MFUnit {
        id: alarmBottomLevel
        width: 54
        height: 20
        objectName: tankUnit.objectName + ".alarmBottomLevel"
        backgroundColor: "#f03e3e"
        tooltip: "Предельный нижний уровень"
        readOnly: false
        visible: true
        anchors.left: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        valueFontSize.bold: false
        disappear: true
        correctingButtons: true
        onValueRealChanged: tankUnit.alarmBottomLevelChanged(valueReal)
        checkLimit: true
        upLimit: alarmTopLevel.valueReal
        downLimit: 0
    }
}
