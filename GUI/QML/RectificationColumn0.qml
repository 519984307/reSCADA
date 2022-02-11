import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13

//import LineComponent 1.0
TankUnit {
    id: root
    width: 70
    height: 350
    property alias pidPTop: pidPTop
    property alias sbPTop: sbPTop

    property alias tOutWater: tOutWater
    property alias pipeOutWtr1: pipeOutWtr1
    //property alias tank: tank
    property int colderDiametr: width * 0.7

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
    mainGradientColor:"#d3d3d3"
    //backgroundColor: "#d3d3d3"
    //Tank {
    //   id: tank
    //   width: 60
    radius: 10
    //objectName:  "cube"
    //anchors.fill: parent
    level: 90
    levelRatio: 0.2
    //    mainGradientColor: backgroundCurrentColor
    //    borderWidth: parent.borderWidth
    //    borderColor: borderCurrentColor
    // }

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
//    MouseArea {
//        id: mousAr
//        anchors.fill: parent
//        acceptedButtons: Qt.RightButton | Qt.LeftButton
//        onClicked: {
//            if (mouse.button & Qt.RightButton) {
//                openSettings()
//            }
//        }
//    }
    PID_Win{
        id: pidPTop
        title: "ПИД " + "давление верха " + root.name
        processName: "Давление верха " + root.name
        impactName: "Положение клапана охлаждающей воды"
        objectName:  "pTopPID"
        colorImpact: pipeOutWaterColor
        colorProcess: "yellow"
        impIsOut: false
        mfuToProcess.valueReal: 3
        mfuImpact.separCorrButtons: true
        mfuKpOut.visible: false
        mfuKiOut.visible:false
        mfuKdOut.visible:false
        kdRow.visible: false
        onManOnChanged: sbPTop.checked = manOn
        Component.onCompleted: {
            sbPTop.checked = manOn
        }
    }
    AnalogSignalVar2 {
        id: tTop
        objectName: "tTop"
        height: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 4
        anchors.topMargin: 53
        anchors.leftMargin: -19
        backgroundColor: "transparent"
        colorShortName: "green"
        valueText: "999.9"
        tooltipText: "Температура верха колонны"
    }
    AnalogSignalVar2 {
        id: pTop
        objectName:  "pTop"
        width: 60
        height: 20
        anchors.left: parent.left
        anchors.top: tTop.bottom
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.leftMargin: -19
        anchors.topMargin: 4
        backgroundColor: "transparent"
        colorShortName: "orange"
        shortNameText: "P"
        valueText: "999.9"
        tooltipText: "Давление верха колонны"
        SimpleButton{
            id: sbPTop
            radius: height / 2
            width: parent.height
            height: parent.height
            checkable: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.left
            anchors.rightMargin: width / 5
            pressCheckColor: "gray"
            unPressCheckColor: "#8afda6"
            mouseArea.onClicked:{
                if( mouse.button & Qt.RightButton ){
                    pidPTop.show()
                }
            }
            onCheckedChanged: pidPTop.manOn = checked
        }
    }
    AnalogSignalVar2 {
        id: tBottom
        objectName:  "tBottom"
        y: 240
        width: 60
        height: 20
        anchors.left: parent.left
        anchors.bottom: pBottom.top
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.leftMargin: -19
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
        colorImpact: pipeSteamColor
        colorProcess: "yellow"
        impIsOut: false
        mfuToProcess.valueReal: 3
        mfuImpact.separCorrButtons: true
        mfuKpOut.visible: false
        mfuKiOut.visible: false
        mfuKdOut.visible: false
        kdRow.visible: false
        onManOnChanged: { sbPrButt.checked = manOn }
        Component.onCompleted: sbPrButt.checked = manOn
    }
    AnalogSignalVar2 {
        id: pBottom
        objectName:  "pBottom"
        y: 259
        width: 60
        height: 20
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.bottomMargin: 76
        anchors.leftMargin: -19
        backgroundColor: "transparent"
        colorShortName: "orange"
        shortNameText: "P"
        valueText: "999.9"
        tooltipText: "Давление низа колонны"
        SimpleButton{
            id: sbPrButt
            radius: height / 2
            width: parent.height
            height: parent.height
            checkable: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.left
            anchors.rightMargin: width / 5
            pressCheckColor: "gray"
            unPressCheckColor: "#8afda6"
            mouseArea.onClicked:{
                if( mouse.button & Qt.RightButton ){
                    pidPBott.show()
                }
            }
            onCheckedChanged: { pidPBott.manOn = checked }
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
        x: -52
        y: -10
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
        width: 60
        objectName:  "tWater"
        height: 20
        anchors.verticalCenter: pipeOutWtr01.verticalCenter
        anchors.left: pipeOutWtr01.horizontalCenter
        anchors.leftMargin: -10
        colorShortName: "#008000"
        tooltipText: "Температура отходящей воды"
        valueText: "99.9"
        //shortNameText: "T"
    }
}




/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/
