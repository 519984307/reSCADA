import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13

//import LineComponent 1.0
UnitPropItem {
    id: root
    width: 70
    height: 350
    property alias pidPTop: pidPTop
    property alias sbPTop: sbPTop

    property alias tOutWater: tOutWater
    property alias pipeOutWtr1: pipeOutWtr1
    property alias tank: tank
    //property int pipeThin: 3

//    property alias tempOutWater:         tOutWater.valueText
//    property alias tempOutWaterAlarmMax: tOutWater.valueMaxLvl
//    property alias tempOutWaterAlarmMin: tOutWater.valueMinLvl

//    property alias tempTop:         tTop.valueText
//    property alias tempTopAlarmMax: tTop.valueMaxLvl
//    property alias tempTopAlarmMin: tTop.valueMinLvl

//    property alias pressTop:         pTop.valueText
//    property alias pressTopAlarmMax: pTop.valueMaxLvl
//    property alias pressTopAlarmMin: pTop.valueMinLvl

//    property alias tempBottom:         tBottom.valueText
//    property alias tempBottomAlarmMax: tBottom.valueMaxLvl
//    property alias tempBottomAlarmMin: tBottom.valueMinLvl

//    property alias pressBottom:         pTop.valueText
//    property alias pressBottomAlarmMax: pBottom.valueMaxLvl
//    property alias pressBottomAlarmMin: pBottom.valueMinLvl

    property int valveNameSize: 10
    property int pipePassThin: 4
    property color pipePassColor: "white"
    property int pipePassBorderWidth: 1

    property int pipeSteamThin: 5
    property color pipeSteamColor: "white" //element.colorSteam
    property int pipeOutWaterThin: 5
    property color pipeOutWaterColor: "white" //element.colorCoolWoter
    property int pipeBorderWidth: 1

    property bool fullView: true
    //    //property alias PIDpTopOn:pidPTop.
    //    property alias PIDpTopShow: pidPTop.visible

//    function setLevel(value) { level = value}

//    function setTempOutWater(value)         { tempOutWater         = value}
//    function setTempOutWaterAlarmMax(value) { tempOutWaterAlarmMax = value}
//    function setTempOutWaterAlarmMin(value) { tempOutWaterAlarmMin = value}

//    function setTempTop(value)         { tempTop = value}
//    function setTempTopAlarmMax(value) { tempTopAlarmMax = value}
//    function setTempTopAlarmMin(value) { tempTopAlarmMin = value}

//    function setPressTop(value)         { pressTop = value}
//    function setPressTopAlarmMax(value) { pressTopAlarmMax = value}
//    function setPressTopAlarmMin(value) { pressTopAlarmMin = value}

//    function setTempBottom(value)         { tempBottom = value}
//    function setTempBottomAlarmMax(value) { tempBottomAlarmMax = value}
//    function setTempBottomAlarmMin(value) { tempBottomAlarmMin = value}

//    function setPessBottom(value)         { pressBottom = value}
//    function setPessBottomAlarmMax(value) { pressBottomAlarmMax = value}
//    function setPessBottomAlarmMin(value) { pressBottomAlarmMin = value}

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
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: {
            if (mouse.button & Qt.RightButton) {
                openSettings()
            }
        }
    }
    Tank {
        id: tank
        radius: 10
        objectName:  "cube"
        anchors.fill: parent
        level: 90
        levelRatio: 0.2
        mainGradientColor: backgroundCurrentColor
        borderWidth: borderWidth
        borderColor: borderCurrentColor
    }
    PID_Win{
        id: pidPTop
        title: "ПИД " + "давление верха " + root.name
        processName: "Давление верха " + root.name
        impactName: "Положение клапана охлаждающей воды"
        objectName:  "pTopPID"
        onIsOnChanged: sbPTop.check = isOn
        Component.onCompleted: {
            sbPTop.check = isOn
        }
    }
    AnalogSignalVar2 {
        id: tTop
        objectName: "tTop"
        width: 60
        height: 15
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 55
        anchors.leftMargin: -14
        backgroundColor: "transparent"
        colorShortName: "green"
        valueText: "999.9"
        tooltipText: "Температура верха колонны"
    }
    AnalogSignalVar2 {
        id: pTop
        objectName:  "pTop"
        width: 60
        height: 15
        anchors.left: parent.left
        anchors.top: tTop.bottom
        anchors.leftMargin: -14
        anchors.topMargin: 4
        backgroundColor: "transparent"
        colorShortName: "orange"
        shortNameText: "P"
        valueText: "999.9"
        tooltipText: "Давление верха колонны"
        SimpleButton{
            id: sbPTop
            radius: height / 2
            width: 16
            height: 16
            checkable: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.left
            anchors.rightMargin: width / 5
            pressCheckColor: "#8afda6"
            mouseArea.onClicked:{
                if( mouse.button & Qt.RightButton ){
                    pidPTop.show()
                }
            }
            onCheckChanged: pidPTop.isOn = check
        }
    }
    AnalogSignalVar2 {
        id: tBottom
        objectName:  "tBottom"
        y: 240
        width: 60
        height: 15
        anchors.left: parent.left
        anchors.bottom: pBottom.top
        anchors.leftMargin: -14
        anchors.bottomMargin: 4
        backgroundColor: "transparent"
        colorShortName: "green"
        valueText: "999.9"
        tooltipText: "Температура низа колонны"
    }

    PID_Win{
        id: pidPBott
        title: "ПИД " + "давление низа " + root.name
        processName: "Давление в кубе " + root.name
        impactName: "Положение клапана пара"
        objectName:  "pBottomPID"
        onIsOnChanged: { sbPrButt.check = isOn }
        Component.onCompleted: sbPrButt.check = isOn
    }
    AnalogSignalVar2 {
        id: pBottom
        objectName:  "pBottom"
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
        SimpleButton{
            id: sbPrButt
            radius: height / 2
            width: 16
            height: 16
            checkable: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.left
            anchors.rightMargin: width / 5
            pressCheckColor: "#8afda6"
            mouseArea.onClicked:{
                if( mouse.button & Qt.RightButton ){
                    pidPBott.show()
                }
            }
            onCheckChanged: { pidPBott.isOn = check }
        }
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
        nActiveColor: root.pipeOutWaterColor
    }
    Pipe {
        id: pipeOutWtr1
        x: -67
        y: -11
        width: 57
        height: pipeOutWaterThin
        nActiveColor: pipeOutWaterColor
        borderWidth: pipeBorderWidth
    }

    Pipe {
        id: pipeOutWtr01
        width: pipeOutWaterThin
        height: 20
        anchors.left: pipeOutWtrAng0.left
        anchors.top: pipeOutWtrAng0.bottom
        anchors.leftMargin: 0
        anchors.topMargin: 0
        nActiveColor: pipeOutWaterColor
        borderWidth: pipeBorderWidth
        horOrVert: false
    }

    PipeAngle90 {
        id: pipeOutWtrAng0
        width: 7
        height: 7
        anchors.right: pipeOutWtr1.left
        anchors.top: pipeOutWtr1.top
        anchors.rightMargin: 0
        nActiveColor: pipeOutWaterColor
        pipeThin: pipeOutWaterThin
        borderWidth: pipeBorderWidth
        rotation: 180
    }
    AnalogSignalVar2 {
        id: tOutWater
        objectName:  "tWater"
        width: 60
        height: 15
        anchors.verticalCenter: pipeOutWtr01.verticalCenter
        anchors.left: pipeOutWtr01.horizontalCenter
        anchors.leftMargin: -8
        colorShortName: "#008000"
        tooltipText: "Температура отходящей воды"
        valueText: "999.9"
        //shortNameText: "T"
    }
}




/*##^##
Designer {
    D{i:0;formeditorZoom:1.5}
}
##^##*/
