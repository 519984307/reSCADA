import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13

//import LineComponent 1.0
UnitPropItem {
    id: rc0
    width: 70
    height: 350
    property alias pipeOutWtr1: pipeOutWtr1
    rotation: 0
    property alias tempOutWater: tempOutWater
    property alias tank: tank
    //property int pipeThin: 3
    property int valveNemeSize: 10
    property int pipePassThin: 4
    property color pipePassColor: "white"
    property int pipePassBorderWidth: 1

    property int pipeSteamThin: 5
    property color pipeSteamColor: "white" //element.colorSteam
    property int pipeOutWaterThin: 5
    property color pipeOutWaterColor: "white" //element.colorCoolWoter
    property int pipeBorderWidth: 1

    function setTempTop(value) {
        tempTop.setValue(value)
    }
    function setAlarmTempTop(value) {
        alarmTempTop.setAlarmMaxLvl(value)
    }
    signal alarmTempTopChanged(variant value)

    function setPresTop(value) {
        presTop.setValue(value)
    }
    function setAlarmPresTop(value) {
        presTop.setAlarmMaxLvl(value)
    }
    signal alarmPresTopChanged(variant value)

    function setTempButtom(value) {
        tempButtom.setValue(value)
    }
    function setAlarmTempButtom(value) {
        tempButtom.setAlarmMaxLvl(value)
    }
    signal alarmTempButtomChanged(variant value)

    function setPresButtom(value) {
        presButtom.setValue(value)
    }
    function setAlarmPresButtom(value) {
        presButtom.setAlarmMaxLvl(value)
    }
    signal alarmPresButtomChanged(variant value)

    function setOutWaterTemp(value) {
        tempOutWater.setValue(value)
    }
    function setAlarmOutWaterTemp(value) {
        tempOutWater.setAlarmMaxLvl(value)
    }
    signal alarmOutWaterTempChanged(variant value)

    backgroundColor: "#d3d3d3"

    //++++++++ Test +++++++
    //    mouseArea.onPressAndHold: {
    //        linked = true
    //        connected = true
    //        allovAlarmBlinck = true
    //        setQuitAlarm()
    //        var cl = tank.mainGradientColor
    //        cl = backgroundCurrentColor
    //    }
    //------ Test ------
    MouseArea {
        id: mousAr
        anchors.fill: parent
        acceptedButtons: Qt.AllButtons
        onClicked: {
            if (mouse.button & Qt.RightButton) {
                openSettings()
            }
        }
    }
    Tank {
        id: tank
        radius: 10
        objectName: rc0.objectName + ".tank"
        anchors.fill: parent
        level: 0.9
        levelRatio: 0.2
        mainGradientColor: backgroundCurrentColor
        borderWidth: borderWidth
        borderColor: borderCurrentColor
    }
    AnalogSignalVar2 {
        id: tempTop
        objectName: rc0.objectName + ".tempTop"
        width: 60
        height: 15
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 44
        anchors.leftMargin: -14
        backgroundColor: "transparent"
        colorShortName: "green"
        shortNameText: "T"
        valueText: "999.9"
        tooltipText: "Температура верха колонны"
        onAlarmMaxLvlChanged: rc0.alarmTempTopChanged(value)
    }
    AnalogSignalVar2 {
        id: presTop
        objectName: parent.objectName + ".presTop"
        width: 60
        height: 15
        anchors.left: parent.left
        anchors.top: tempTop.bottom
        anchors.leftMargin: -14
        anchors.topMargin: 4
        backgroundColor: "transparent"
        colorShortName: "orange"
        shortNameText: "P"
        valueText: "999.9"
        tooltipText: "Давление верха колонны"
        onAlarmMaxLvlChanged: rc0.alarmPresTopChanged(value)
    }
    AnalogSignalVar2 {
        id: tempButtom
        objectName: parent.objectName + ".tempButtom"
        y: 240
        width: 60
        height: 15
        anchors.left: parent.left
        anchors.bottom: presButtom.top
        anchors.leftMargin: -14
        anchors.bottomMargin: 4
        backgroundColor: "transparent"
        colorShortName: "green"
        shortNameText: "T"
        valueText: "999.9"
        tooltipText: "Температура низа колонны"
        onAlarmMaxLvlChanged: rc0.alarmTempButtomChanged(value)
    }
    AnalogSignalVar2 {
        id: presButtom
        objectName: parent.objectName + ".presButtom"
        y: 259
        width: 60
        height: 15
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 76
        anchors.leftMargin: -14
        backgroundColor: "transparent"
        colorShortName: "orange"
        shortNameText: "P"
        valueText: "999.9"
        tooltipText: "Давление низа колонны"
        onAlarmMaxLvlChanged: rc0.alarmPresButtomChanged(value)
    }

    Triangle {
        x: -31
        width: pipeOutWaterThin * 2
        height: width
        anchors.top: pipeOutWtr01.bottom
        anchors.horizontalCenter: pipeOutWtr01.horizontalCenter
        anchors.topMargin: 0
        rotation: 0
        borderWidth: pipeBorderWidth
        nActivColor: rc0.pipeOutWaterColor
    }
    Pipe {
        id: pipeOutWtr1
        x: -67
        y: -11
        width: 57
        height: pipeOutWtrThin
        anchors.right: parent.horizontalCenter
        nActivColor: pipeOutWaterColor
        borderWidth: pipeBorderWidth
        anchors.rightMargin: 0
    }

    PipeAngle {
        id: pipeOutWtrAng0
        width: 7
        height: 7
        anchors.right: pipeOutWtr1.left
        anchors.top: pipeOutWtr1.top
        anchors.rightMargin: 0
        nActivColor: pipeOutWaterColor
        pipeThin: pipeOutWaterThin
        borderWidth: pipeBorderWidth
        rotation: 180
        z: 0
    }

    Pipe {
        id: pipeOutWtr01
        width: pipeOutWaterThin
        height: 20
        anchors.left: pipeOutWtrAng0.left
        anchors.top: pipeOutWtrAng0.bottom
        anchors.leftMargin: 0
        anchors.topMargin: 0
        nActivColor: pipeOutWaterColor
        borderWidth: pipeBorderWidth
        horOrVert: false
    }
    AnalogSignalVar2 {
        id: tempOutWater
        objectName: parent.objectName + ".tempOutWater"
        width: 60
        height: 15
        anchors.verticalCenter: pipeOutWtr01.verticalCenter
        anchors.left: pipeOutWtr01.horizontalCenter
        anchors.leftMargin: -8
        colorShortName: "#008000"
        tooltipText: "Температура отходящей воды"
        valueText: "999.9"
        shortNameText: "T"
        onAlarmMaxLvlChanged: alarmOutWaterTempChanged(value)
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:4}
}
##^##*/

