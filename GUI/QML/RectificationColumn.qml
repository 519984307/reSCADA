import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13
import LineComponent 1.0

UnitItem{
    id: root
    width: 70
    height: 350

    property alias tank: tank

    function setTempTop( value ) { tempTop.setValue( value ) }
    function setAlarmTempTop( value ) { alarmTempTop.setLableSilent( value ) }
    signal alarmTempTopChanged( variant value )

    function setPresTop( value ) { presTop.setValue( value ) }
    function setAlarmPresTop( value ) { alarmPresTop.setLableSilent( value ) }
    signal alarmPresTopChanged( variant value )

    function setTempButtom( value ) { tempButtom.setValue( value ) }
    function setAlarmTempButtom( value ) { alarmTempButtom.setLableSilent( value ) }
    signal alarmTempButtomChanged( variant value )

    function setPresButtom( value ) { presButtom.setValue( value ) }
    function setAlarmPresButtom( value ) { alarmPresButtom.setLableSilent( value ) }
    signal alarmPresButtomChanged( variant value )
    backgroundColor: "#d3d3d3"

    linked: true
    connected: true

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
    Tank {
        id: tank
        objectName: root.objectName + ".tank"
        anchors.fill: parent
        level: 0.9
        levelRatio: 0.2
        mainGradientColor: backgroundCurrentColor
        borderWidth: borderWidth
        borderColor: borderCurrentColor
        contentGradientColor: "#a65121"
    }
    AnalogSignalVar1{
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
        //blinkColor: "yellow"
        nameText: "T"
        valueText: "999.9"
        tooltipText:"Температура верха колонны"

        MFUnit {
            id: alarmTempTop
            objectName: root.objectName + ".alarmTempTop"
            width: parent.width
            height: parent.height
            anchors.left: parent.right

            backgroundColor: "#f03e3e"
            tooltip: "Температура перегрева верха колонны"
            readOnly: false
            visible: true
            valueFontSize.bold: false
            valueFontSize.family: "DSEG7 Classic"
            disappear: true
            valueFontSize.pointSize: 12
            blinkColor: "#ff0000"
            correctingButtons: true
            regexp: floatreg
            onValueChanged: root.alarmTempTopChanged(value)
            checkLimit: true
            downLimit: 20
        }
    }
    AnalogSignalVar1{
        id:presTop
        width: 80
        height: 20
        anchors.left: parent.left
        anchors.top: tempTop.bottom
        anchors.leftMargin: -19
        anchors.topMargin: 4
        backgroundColor: "transparent"
        colorShortName: "orange"
        nameText: "P"
        valueText:"999.9"
        tooltipText:"Давление верха колонны"


        MFUnit {
            id: alarmPresTop
            objectName: root.objectName + ".alarmPresTop"
            width: parent.width
            height: parent.height
            anchors.left: parent.right

            backgroundColor: "#f03e3e"
            tooltip: "Предельное давление верха колонны"
            readOnly: false
            visible: true
            valueFontSize.bold: false
            valueFontSize.family: "DSEG7 Classic"
            disappear: true
            valueFontSize.pointSize: 12
            blinkColor: "#ff0000"
            correctingButtons: true
            regexp: floatreg
            onValueChanged: root.alarmPresTopChanged(value)
            checkLimit: true
            downLimit: 20
        }
    }

    AnalogSignalVar1{
        id:tempButtom
        width: 80
        height: 20
        anchors.left: parent.left
        anchors.bottom: presButtom.top
        anchors.leftMargin: -19
        anchors.bottomMargin: 4
        backgroundColor: "transparent"
        colorShortName: "green"
        nameText: "T"
        valueText:"999.9"
        tooltipText:"Температура низа колонны"

        MFUnit {
            id: alarmTempButtom
            objectName: root.objectName + ".alarmTempButtom"
            width: parent.width
            height: parent.height
            anchors.left: parent.right

            backgroundColor: "#f03e3e"
            tooltip: "Температура перегрева верха колонны"
            readOnly: false
            visible: true
            valueFontSize.bold: false
            valueFontSize.family: "DSEG7 Classic"
            disappear: true
            valueFontSize.pointSize: 12
            blinkColor: "#ff0000"
            correctingButtons: true
            regexp: floatreg
            onValueChanged: root.alarmTempButtomChanged(value)
            checkLimit: true
            downLimit: 20
        }
    }
    AnalogSignalVar1{
        id:presButtom
        width: 80
        height: 20
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: -19
        anchors.bottomMargin: parent.height/8
        backgroundColor: "transparent"
        colorShortName: "orange"
        nameText: "P"
        valueText:"999.9"
        tooltipText:"Давление низа колонны"
        //Component.onCompleted: {renewColors()}

        linked: true
        connected: true

        MFUnit {
            id: alarmPresButtom
            objectName: root.objectName + ".alarmPresButtom"
            width: parent.width
            height: parent.height
            anchors.left: parent.right

            backgroundColor: "#f03e3e"
            valueText:"999.9"
            tooltip: "Предельное давление низа колонны"
            readOnly: false
            visible: true
            valueFontSize.bold: false
            valueFontSize.family: "DSEG7 Classic"
            disappear: true
            valueFontSize.pointSize: 12
            blinkColor: "#ff0000"
            correctingButtons: true
            regexp: floatreg
            onValueChanged: root.alarmPresButtomChanged(value)
            checkLimit: true
            downLimit: 20
        }
    }

}






/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/
