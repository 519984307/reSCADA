import QtQuick 2.15
//import QtQuick.Controls 2.15
import "fap.js" as Fap

Item {
    id: unit
    property string name: "NoName"
    property string description: "Unknown"
    property string alarmDescr: "Unknown"

    property bool alarm: false
    property bool alarmNotify: false
    property bool notify: false
    property bool allovAlarmBlinck: true

    property bool blocked: false
    property bool linked: false
    property bool connected: false

    property color backgroundCurrentColor: Fap.defaultColor
    property color backgroundColor: Fap.ready
    property color backgroundAlarmColor: Fap.alarm
    property color backgroundAlarmBlinkColor: Fap.alarm

    property int borderWidth: 2
    property color borderCurrentColor: Fap.border
    property color borderColor: Fap.border
    property color borderAlarmColor: Fap.borderAlarm
    property color borderNotifyBlincColor: Fap.borderNotify

    //    Component.onCompleted: {
    //        linkColors()
    //        renewColors()
    //    }
    signal resetAlarm()
    //    function linkColors() {
    //        //backgroundColor = backgroundCurrentColor
    //        //borderColor = borderCurrentColor
    //    }
    //    function setAlarm() {
    //        alarm = true
    //    }
    function setAlarmComb( Descr ) {
        if(Descr == ""){
            setAlarm( alarmDescr )
        }
        else{
            setQuitAlarm( Descr )
        }
    }
    function setAlarm( Descr ) {
        if(Descr != ""){
            alarmDescr = Descr
        }
        alarmNotify = false
        alarm = true
    }
    function setQuitAlarm( Descr ) {
        setAlarm( Descr )
        alarmNotify = true
    }
    function alarmReseted() {
        notify = false
        alarm = false
        alarmNotify = false
        alarmDescr = ""
    }
    function setNotify() {
        //вкл мигание
        notify = true
    }
    function setNotified() {
        //прекр мигание
        notify = false
    }
    function setLinked() {
        //        linkColors()
        linked = true
    }
    function setConnected() {
        connected = true
    }
    function setDisconnected() {
        connected = false
    }
    function setName(val) {
        name = val
    }

    function renewColors() {
        //        var St
        //        St = String(borderCurrentColor)
        if (!linked) {
            timer.stop()
            backgroundCurrentColor = Fap.notAvailable
            borderCurrentColor = Fap.notAvailable
            return
        }
        else if (!connected) {
            backgroundCurrentColor = Fap.notAvailable
        }
        else if (blocked) {
            backgroundCurrentColor = Fap.blocked
        }
        else
            backgroundCurrentColor = backgroundColor

        if (alarm || alarmNotify) {
            borderCurrentColor = borderAlarmColor
        }
        else {
            borderCurrentColor = borderColor
        }
    }

    Timer {
        id: timer
        interval: Fap.blinkDelay
        running: false
        repeat: true
        onTriggered: {
            if (allovAlarmBlinck && alarmNotify) {
                backgroundCurrentColor = backgroundCurrentColor
                        == backgroundColor ? backgroundAlarmBlinkColor : backgroundColor
            }
            if (notify) {
                if (alarm) {
                    borderCurrentColor = borderCurrentColor
                            == borderAlarmColor ? borderNotifyBlincColor : borderAlarmColor
                } else {
                    borderCurrentColor = borderCurrentColor
                            == borderColor ? borderNotifyBlincColor : borderColor
                }
            }
        }
    }
    onAlarmChanged: renewColors()

    onAlarmNotifyChanged: {
        renewColors()
        if (alarmNotify && allovAlarmBlinck)
            timer.start()
        else if (!notify)
            timer.stop()
    }

    onAllovAlarmBlinckChanged: {
        renewColors()
        onAlarmNotifyChanged()
    }
    onNotifyChanged: {
        renewColors()
        if (notify)
            timer.start()
        else if (!alarmNotify)
            timer.stop()
    }
    onBlockedChanged: renewColors()
    onLinkedChanged: renewColors()
    onConnectedChanged: renewColors()
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

