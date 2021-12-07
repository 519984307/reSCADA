import QtQuick 2.12
//import QtQuick.Controls 1.3
import QtQuick.Controls 2.12
import "fap.js" as Fap

AnalogSignalVar1{
    signal alarmMaxLvlChanged( variant value )
    signal alarmMinLvlChanged( variant value )
    function setAlarmMaxLvl( value ){
        maxLvl.setValue(value)
    }
    function setAlarmMinLvl( value ){
        minLvl.setValue(value)
    }

    MFUnit {
        id: maxLvl
        width: parent.width
        height: parent.height
        anchors.left: parent.right
        backgroundColor: "#c36b6b"
        tooltip: "Max"
        readOnly: false
        visible: true
        disappear: true
        correctingButtons: true
        onValueChanged: alarmMaxLvlChanged(value)
        checkLimit: true
    }
    MFUnit {
        id: minLvl
        width: parent.width
        height: parent.height
        anchors.right: parent.left
        backgroundColor: "#6e9ec8"
        tooltip: "Min"
        readOnly: false
        visible: true
        disappear: true
        correctingButtons: true
        onValueChanged: alarmMinLvlChanged(value)
        checkLimit: true
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}
}
##^##*/
