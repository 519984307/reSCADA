import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13
import LineComponent 1.0

UnitPropItem{
    id: root
    width: 70
    height: 350

    property alias tank: tank

    property int tempOutWtrVert: 5
    property int tempOutWtrHor: -86

    function setTempTop( value ) { tempTop.setValue( value ) }
    function setAlarmTempTop( value ) { alarmTempTop.setAlarmMaxLvl( value ) }
    signal alarmTempTopChanged( variant value )

    function setPresTop( value ) { presTop.setValue( value ) }
    function setAlarmPresTop( value ) { presTop.setAlarmMaxLvl( value ) }
    signal alarmPresTopChanged( variant value )

    function setTempButtom( value ) { tempButtom.setValue( value ) }
    function setAlarmTempButtom( value ) { tempButtom.setAlarmMaxLvl( value ) }
    signal alarmTempButtomChanged( variant value )

    function setPresButtom( value ) { presButtom.setValue( value ) }
    function setAlarmPresButtom( value ) { presButtom.setAlarmMaxLvl( value ) }
    signal alarmPresButtomChanged( variant value )

    function setOutWaterTemp( value ) { tempOutWater.setValue( value ) }
    function setAlarmOutWaterTemp( value ) { tempOutWater.setAlarmMaxLvl( value ) }
    signal alarmOutWaterTempChanged( variant value )

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

    MouseArea{
        id: mousAr
        anchors.fill: parent
        acceptedButtons: Qt.AllButtons
        onClicked: {
            if(mouse.button & Qt.RightButton) {
                openSettings()
            }
        }
    }
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
    AnalogSignalVar2{
        id:tempTop
        objectName: root.objectName + ".tempTop"
        width: 80
        height: 20
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: -19
        anchors.topMargin: parent.height/8
        backgroundColor: "transparent"
        colorShortName: "green"
        shortNameText: "T"
        valueText: "999.9"
        tooltipText:"Температура верха колонны"
        onAlarmMaxLvlChanged: root.alarmTempTopChanged(value)
    }
    AnalogSignalVar2{
        id:presTop
        width: 80
        height: 20
        anchors.left: parent.left
        anchors.top: tempTop.bottom
        anchors.leftMargin: -19
        anchors.topMargin: 4
        backgroundColor: "transparent"
        colorShortName: "orange"
        shortNameText: "P"
        valueText:"999.9"
        tooltipText:"Давление верха колонны"
        onAlarmMaxLvlChanged: root.alarmPresTopChanged(value)
    }
    AnalogSignalVar2{
        id:tempButtom
        width: 80
        height: 20
        anchors.left: parent.left
        anchors.bottom: presButtom.top
        anchors.leftMargin: -19
        anchors.bottomMargin: 4
        backgroundColor: "transparent"
        colorShortName: "green"
        shortNameText: "T"
        valueText:"999.9"
        tooltipText:"Температура низа колонны"
        onAlarmMaxLvlChanged:root.alarmTempButtomChanged(value)
    }
    AnalogSignalVar2{
        id:presButtom
        width: 80
        height: 20
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: -19
        anchors.bottomMargin: parent.height/8
        backgroundColor: "transparent"
        colorShortName: "orange"
        shortNameText: "P"
        valueText:"999.9"
        tooltipText:"Давление низа колонны"
        onAlarmMaxLvlChanged:root.alarmPresButtomChanged( value )
    }
    AnalogSignalVar2 {
        id: tempOutWater
        y: -45
        width: 80
        height: 20
        anchors.bottom: parent.top
        anchors.bottomMargin: tempOutWtrVert
        anchors.horizontalCenterOffset: tempOutWtrHor
        anchors.horizontalCenter: parent.horizontalCenter
        backgroundColor: "#ffffff"
        colorShortName: "#008000"
        tooltipText: "Температура отходящей воды"
        valueText: "999.9"
        shortNameText: "T"
        onAlarmMaxLvlChanged: alarmOutWaterTempChanged( value )
    }
}



/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/
