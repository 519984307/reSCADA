import QtQuick 2.12

UnitItem {
    id: root
    width: 70
    height: 350

    property alias tank: tank
    backgroundColor: "#d3d3d3"

    function setLevel(value) {
        tempTop.setValue(value)
    }

    function setAlarmLevelTop(value) {
        alarmTempTop.setLableSilent(value)
    }
    signal alarmTopLevelChanged(variant value)

    function setAlarmLevelButtom(value) {
        alarmTempTop.setLableSilent(value)
    }
    signal alarmButtomLevelChanged(variant value)

    //++++++++ Test +++++++
    //        mouseArea.onPressAndHold: {
    //            linked = true
    //            connected = true
    //            allovAlarmBlinck = true
    //            setQuitAlarm()
    //            var cl = tank.mainGradientColor
    //            cl = backgroundCurrentColor
    //        }
    // ------ Test ------
    UnitItem {}
    Tank {
        id: tank
        objectName: root.objectName + ".tank"
        anchors.fill: parent
        level: 0.9
        levelRatio: 0.2
        mainGradientColor: backgroundCurrentColor
        borderWidth: borderWidth
        borderColor: borderCurrentColor
    }

    MFUnit {
        id: alarmTopLevel
        width: 54
        objectName: root.objectName + ".alarmTopLevel"
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
        onValueChanged: root.alarmTopLevelChanged(value)
        checkLimit: true
        downLimit: 20
    }
    MFUnit {
        id: alarmButtomLevel
        width: 54
        height: 20
        objectName: root.objectName + ".alarmButtomLevel"
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
        onValueChanged: root.alarmButtomLevelChanged(value)
        checkLimit: true
        downLimit: 20
    }
}
