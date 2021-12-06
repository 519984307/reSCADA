import QtQuick 2.15
import QtQuick.Controls 2.15
import "fap.js" as Fap

Item {
    id: unitItem
    width: 100
    height: 100
    visible: true


    //property int canvasOffset: 3
    property string name: "Unknown Unit"
    property string description: "Unknown Unit"
    property alias tooltipText: ttip.text

    property alias rotate: unitItem.rotation
    //property Font  font: ({})
    property  alias mouseArea: mouseArea
    property  alias intreg: intreg
    property  alias floatreg: floatreg

    property bool alarm:            false
    property bool alarmNotify:      false
    property bool notify:           false
    property bool allovAlarmBlinck: true

    property bool blocked:   false
    property bool linked:    false
    property bool connected: false
    property bool disappear: false


    property color backgroundCurrentColor: Fap.defaultColor
    property color backgroundColor: Fap.ready
    property color backgroundAlarmColor: Fap.alarm
    property color backgroundAlarmBlinkColor: Fap.alarm

    property int   borderWidth: 2
    property color borderCurrentColor: Fap.border
    property color borderColor: Fap.border
    property color borderAlarmColor: Fap.borderAlarm
    property color borderNotifyBlincColor: Fap.borderNotify

    RegExpValidator {
        id: intreg
        regExp: /[0-9]+/
    }

    RegExpValidator {
        id: floatreg
        regExp: /[0-9]+[.][0-9]/
    }

    Component.onCompleted: {
        renewColors()
    }

    function setAlarm(){
        alarm = true
    }
    function setQuitAlarm(){
        setAlarm()
        alarmNotify = true
    }
    function alarmReseted(){
        alarm = false
        alarmNotify = false
    }
    function setNotify(){//вкл мигание
        notify = true
    }
    function setNotified(){//прекр мигание
        notify = false
    }
    function setLinked(){
        linked = true
    }
    function setConnected(){
        connected = true
    }
    function setDisconnected(){
        connected = false
    }
    function setExName(val){
        name = val
    }

    function renewColors(){
        if( !linked ){
            timer.stop()
            backgroundCurrentColor = Fap.notAvailable
            borderCurrentColor = Fap.notAvailable
            return
        }
        else if( !connected ){
            backgroundCurrentColor = Fap.notAvailable
        }
        else if(blocked){
            backgroundCurrentColor = Fap.blocked
        }
        else
            backgroundCurrentColor = backgroundColor

        if( alarm || alarmNotify ){
            borderCurrentColor = borderAlarmColor
        }
        else{
            var St = String(borderCurrentColor)
            borderCurrentColor = borderColor
            St = String(borderCurrentColor)

        }
    }

    signal resetAlarm()
    Timer {
        id: timer
        interval: Fap.blinkDelay
        running: false
        repeat: true
        onTriggered: {
            if( allovAlarmBlinck && alarmNotify ){
                backgroundCurrentColor = backgroundCurrentColor == backgroundColor ? backgroundAlarmBlinkColor : backgroundColor
            }
            if( notify ){
                if( alarm ){
                    borderCurrentColor = borderCurrentColor == borderAlarmColor ?  borderNotifyBlincColor: borderAlarmColor
                }
                else{
                    borderCurrentColor = borderCurrentColor == borderColor ?  borderNotifyBlincColor: borderColor
                }
            }
        }
    }

    function openSettings(){
        if(prop.visible==false){
            var absolutePos = Fap.getAbsolutePosition(unitItem);
            prop.setX(absolutePos.x)
            prop.setY(absolutePos.y)
            prop.show()
        }
        prop.requestActivate()
    }

    onAlarmChanged: renewColors();

    onAlarmNotifyChanged: {
        renewColors()
        if( alarmNotify && allovAlarmBlinck ) timer.start()
        else if( ! notify ) timer.stop()
    }

    onAllovAlarmBlinckChanged: {
        renewColors()
        onAlarmNotifyChanged()
    }
    onNotifyChanged: {
        renewColors()
        if( notify ) timer.start()
        else if( ! alarmNotify )timer.stop()
    }
    onBlockedChanged: renewColors()
    onLinkedChanged:renewColors()
    onConnectedChanged:renewColors()
    onDisappearChanged:{ visible = disappear }


    //    SequentialAnimation{
    //        id: alarmFlah
    //        loops: Animation.Infinite
    //        running: false
    //        property color startColor: "green"
    //        ColorAnimation {
    //            target: body
    //            property: "color"
    //            to: backgroundAlarmBlinkColor
    //            duration: 1000
    //        }
    //        ColorAnimation {
    //            target: body
    //            property: "color"
    //            to: backgroundColor
    //            duration: 1000
    //        }
    //    }

    PropertyWindow3 {
        id: prop
        objectName: "prop"
        winTitle: name
        onResetAlarm: unitItem.resetAlarm()
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if(mouse.button & Qt.RightButton) {
                unitItem.openSettings()
            }
        }
        onPressAndHold: {
            renewColors()

        }
        onDoubleClicked: {
            unitItem.openSettings()
        }
        onContainsMouseChanged: {
            if( disappear ){
                if( containsMouse )
                    visible = true;
                else
                    visible = false;
            }
        }
    }
    ToolTip{
        id: ttip
        delay: 2000
        timeout: 2000
        visible: false
        text: "Unknown Unit"
    }

}
