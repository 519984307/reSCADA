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
    property bool alarmReact: true
    property bool allovAlarmTextBlinck: true
    property bool allovAlarmBodyBlinck: true
    property bool allovAlarmBorderBlinck: true

    property bool blocked: false
    property bool linked: false
    property bool connected: false

    property color backgroundCurrentColor: Fap.defaultColor
    property color backgroundColor: Fap.ready
    property color backgroundAlarmColor: Fap.alarm
    property color backgroundAlarmBlinkColor: Fap.alarm

    property int   borderWidth: 1
    property int   borderWidthNotify: 3
    property int   borderCurrentWidth: 1
    property color borderCurrentColor: Fap.border
    property color borderColor: Fap.border
    property color borderAlarmColor: Fap.borderAlarm
    property color borderNotifyBlincColor: Fap.borderNotify
    property color textColor: Fap.text
    property color textCurrentColor: Fap.text
    property color textAlarmColor: Fap.textAlarm


    Component.onCompleted: {
        renewColors()
    }
    signal s_resetAlarm()
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
        if(alarmReact)alarmNotify = true
    }
    function alarmReseted() {
        notify = false
        alarm = false
        alarmNotify = false
        alarmDescr = ""
    }
    function setAlarmNotify() {
        //РІРєР» РјРёРіР°РЅРёРµ
        if(alarmReact)alarmNotify = true
    }
    function setAlarmNotified() {
        //РїСЂРµРєСЂ РјРёРіР°РЅРёРµ
        alarmNotify = false
    }
    function setNotify() {
        //РІРєР» РјРёРіР°РЅРёРµ
        notify = true
    }
    function setNotified() {
        //РїСЂРµРєСЂ РјРёРіР°РЅРёРµ
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
            backgroundCurrentColor = Fap.notLinked
            borderCurrentColor = Fap.notLinked
            return
        }
        else if (!connected) {
            backgroundCurrentColor = Fap.notConnected
        }
        else if (blocked) {
            backgroundCurrentColor = Fap.blocked
        }
        else
            backgroundCurrentColor = backgroundColor

        if (alarmReact && (alarm || alarmNotify)) {
            borderCurrentColor = borderAlarmColor
            borderCurrentWidth = borderWidthNotify
        }
        else {
            borderCurrentColor = borderColor
            borderCurrentWidth = borderWidth
        }
    }

    Timer {
        id: timer
        interval: Fap.blinkDelay
        running: false
        repeat: true
        onTriggered: {
            if(alarmNotify){
                if (allovAlarmBodyBlinck) {
                    backgroundCurrentColor = backgroundCurrentColor
                            == backgroundColor ? backgroundAlarmBlinkColor : backgroundColor
                }
                if (allovAlarmBorderBlinck) {
                    borderCurrentColor = borderCurrentColor
                            == borderAlarmColor ? borderColor : borderAlarmColor
                }
                if (allovAlarmTextBlinck) {
                    textCurrentColor = textCurrentColor
                            == textAlarmColor ? textColor : textAlarmColor
                }
            }else if (notify) {
                if (!allovAlarmBorderBlinck) {
                    borderCurrentColor =
                            borderCurrentColor == borderColor ? borderNotifyBlincColor
                                                              : ( (alarm && alarmReact)? borderAlarmColor : borderColor)
                }
            }
        }
    }
    onAlarmChanged: renewColors()
    function aNch(){
        renewColors()
        if (alarmNotify)//&& allovAlarmBodyBlinck)
            timer.start()
        else if (!notify)
            timer.stop()
    }
    onAlarmNotifyChanged: {
        aNch()
    }
    onAllovAlarmBodyBlinckChanged: {
        aNch()
    }
    onAllovAlarmBorderBlinckChanged: {
        aNch()
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
    onBackgroundColorChanged: renewColors()
    onAlarmReactChanged: {
        alarmNotify = alarmReact && alarm
        renewColors()
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
