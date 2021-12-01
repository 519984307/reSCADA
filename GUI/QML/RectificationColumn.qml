import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13
import LineComponent 1.0

Item{
    id: root
    width: 70
    height: 350
    property alias tank: tank
    Tank {
        id: tank
        objectName: root.objectName + ".tank"
        anchors.fill: parent
        level: 0.9
        levelRatio: 0.2
        borderWidth: 2
    }

    function setTempTop( value ) { tempTop.setLableSilent( value ) }
    function setAlarmTempTop( value ) { alarmTempTop.setLableSilent( value ) }
    signal alarmTempTopChanged( variant value )

    function setPresTop( value ) { presTop.setLableSilent( value ) }
    function setAlarmPresTop( value ) { alarmPresTop.setLableSilent( value ) }
    signal alarmPresTopChanged( variant value )

    function setTempButtom( value ) { tempButtom.setLableSilent( value ) }
    function setAlarmTempButtom( value ) { alarmTempButtom.setLableSilent( value ) }
    signal alarmTempButtomChanged( variant value )

    function setPresButtom( value ) { presButtom.setLableSilent( value ) }
    function setAlarmPresButtom( value ) { alarmPresButtom.setLableSilent( value ) }
    signal alarmPresButtomChanged( variant value )

    AnalogSignalVar1{
        id:tempTop
        objectName: root.objectName + ".tempTop"
        width: 80
        height: 20
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: -19
        anchors.topMargin: parent.height/8
        regexp: floatreg
        backgroundColor: "white"
        colorShortName: "green"
        blinkColor: "yellow"
        borderColor: "black"
        nameText: "T"
        valueText:"999.9"
        tooltip:"Температура верха колонны"

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

        backgroundColor: "white"
        colorShortName: "orange"
        blinkColor: "yellow"
        borderColor: "black"
        nameText: "P"
        valueText:"999.9"
        tooltip:"Давление верха колонны"


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


        backgroundColor: "white"
        colorShortName: "green"
        blinkColor: "yellow"
        borderColor: "black"
        nameText: "T"
        valueText:"999.9"
        tooltip:"Температура низа колонны"

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

        backgroundColor: "white"
        colorShortName: "orange"
        blinkColor: "yellow"
        borderColor: "black"
        nameText: "P"
        valueText:"999.9"
        tooltip:"Давление низа колонны"

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
