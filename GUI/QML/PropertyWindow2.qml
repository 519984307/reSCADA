﻿import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml.Models 2.12
import "fap.js" as Fap

Window {
    visible: false
    width: 300
    height: windowContent.height
    id: root
    flags: Qt.Window | Qt.Dialog
    title: winTitle + (route>0?" ("+route+")":"")
    minimumWidth: 200
    maximumHeight: 600
    property string winTitle: ""
    property alias st: buttonStartStop.st
    property alias st1: buttonLO.st
    property alias st2: buttonRC.st
    property alias inSt1: buttonLO.inSt
    property alias inSt2: buttonRC.inSt
    property alias buttonsType: areaHead.buttonsType
    property alias alarm: areaAlarm.alarm
    property alias alarmNotify: areaAlarm.alarmNotify
    property int rowSize: 20
    property int route: 0
    property int current: -1
    property bool available: false
    property bool linked: false

    property string exName: ""
    onExNameChanged: changedExName(exName)

    //new interface begin
    signal changedExName(string val)
    function addAlarm(objname, symname, ignorable){
        alarms.insert(alarms.index, propertyAlarm.createObject(alarms))
        alarms.get(alarms.index).objectName = objname;
        alarms.get(alarms.index).alarmName = symname;
        alarms.get(alarms.index).ignorable = ignorable;
        alarms.index++
        return alarms.get(alarms.index - 1)
    }

    function addEngRow(objname){
        engSignals.insert(engSignals.index, propertyEngRow.createObject(engSignals))
        engSignals.get(engSignals.index).objectName = objname;
        engSignals.index++
        return engSignals.get(engSignals.index - 1)
    }
    function alarmSet(){
        areaAlarm.alarm = true
    }
    function alarmReseted(){
        areaAlarm.alarm = false
        areaAlarm.alarmNotify = false
    }
    function quitAlarm(){
        alarmSet()
        areaAlarm.alarmNotify = true
    }
    function setLinked(){
        root.linked = true
    }
    function connected(){
        root.available = true
    }
    function disconnected(){
        root.available = false
    }
    function setRoute(val){
        root.route = val
    }
    function setExName(val){
        root.exName = val
    }
    function setCurrent(val){
        root.current = val
        if (root.current === -1){
            currentTop.text = "---"
        }else if(root.current < 10){
            currentTop.text = "!!" + root.current
        }else if(root.current < 100){
            currentTop.text = "!" + root.current
        }else if(root.current <1000){
            currentTop.text = root.current
        }else{
            currentTop.text = "err"
        }
    }

    //new interface end

    signal resetAlarm();
    signal start();
    signal start1();
    signal start2();
    signal stop();

    onVisibleChanged: {
        if (!visible){
            areaHidden.hidden = true
            root.height = windowContent.height
            for (var i = 0; i < engSignals.index; i++) {
                var engrow = engSignals.get(i)
                for (var j = 0; j < engrow.engModel.index; j++) {
                    var engrowrow = engrow.engModel.get(j)
                    if (engrowrow.settingName !== undefined){
                        engrowrow.commit()
                    }
                }
            }
        }else{
            areaHidden.hidden = false
        }
    }

    Component{
        id: propertyAlarm
        CheckBox {
            property int rowSize: 20
            property int indicatorSize: 16
            property string alarmName: "Unknown alarm"
            property bool alarm: true
            property bool connected: false
            property bool ignorable: true
            property color colorAlarm: "red"
            property color colorNormal: "white"
            signal changedIgnore(bool status)
            function changeIgnore(status){
                checkState = status?Qt.Checked:Qt.Unchecked
            }
            function changeConnected(status){
                connected = status
            }
            function setAlarm(){
                alarm = true
            }
            function resetAlarm(){
                alarm = false
            }
            width: parent==undefined?0:parent.width
            id: root
            height: rowSize
            indicator.width: indicatorSize
            indicator.height: indicatorSize
            checkable: ignorable
            text: alarmName+(connected?"":"(Нет связи)")
            background: Rectangle {color: parent.alarm?parent.colorAlarm:parent.colorNormal }
            onCheckStateChanged: changedIgnore(checkState==Qt.Checked)
            onIgnorableChanged:  {
                if (!ignorable){
                    indicator.width = 0
                    indicator.height = 0
                }
            }
        }
    }

    Component{
        id: propertyEngRow
        Column {
            width: parent==undefined?0:parent.width
            property real ratio: 0.1
            property int rowSize: 20
            property alias engModel: engmodel
            signal sendProperty(int property, variant value)
            function addPropertySignal(objname, symname){
                engmodel.insert(engmodel.index, propertySignal.createObject(engmodel))
                engmodel.get(engmodel.index).objectName = objname
                engmodel.get(engmodel.index).sigName = symname
                engmodel.index++
                return engmodel.get(engmodel.index - 1)
            }
            function addPropertyValue(objname, symname){
                engmodel.insert(engmodel.index, propertyValue.createObject(engmodel))
                engmodel.get(engmodel.index).objectName = objname
                engmodel.get(engmodel.index).sigName = symname
                engmodel.index++
                return engmodel.get(engmodel.index - 1)
            }
            function addPropertySetting(objname, symname, tr){
                engmodel.insert(engmodel.index, propertySetting.createObject(engmodel))
                engmodel.get(engmodel.index).objectName = objname
                engmodel.get(engmodel.index).settingName = symname
                engmodel.get(engmodel.index).reversed = tr>0?true:false
                engmodel.get(engmodel.index).timed = tr>=0?true:false
                engmodel.index++
                return engmodel.get(engmodel.index - 1)
            }
            Component{
                id: propertySignal
                Row{
                    property bool val: false
                    property bool connected: false
                    property alias text: vall.text
                    property real ratio: 0.1
                    property int rowSize: 20
                    property int indicatorSize: 16
                    property string sigName: "Unknown signal"
                    width: parent==undefined?0:parent.width
                    height: rowSize
                    signal changedIm(bool status)
                    signal changedImVal(variant status)
                    function changeIm(status){
                        imit.checked = status?Qt.Checked:Qt.Unchecked
                    }
                    function changeImVal(status){
                        imitVal.checked = status?Qt.Checked:Qt.Unchecked
                    }
                    function changeVal(status){
                        val = status?Qt.Checked:Qt.Unchecked
                    }
                    function changeConnected(status){
                        connected = status
                    }
                    CheckBox {
                        id: imit
                        //                        width: parent.width*parent.ratio
                        width: parent.height
                        height: parent.height
                        indicator.width: parent.indicatorSize
                        indicator.height: parent.indicatorSize
                        onCheckStateChanged: changedIm(checkState==Qt.Checked)
                    }
                    CheckBox {
                        id: imitVal
                        //                        width: parent.width*parent.ratio
                        width: parent.height
                        height: parent.height
                        indicator.width: parent.indicatorSize
                        indicator.height: parent.indicatorSize
                        onCheckStateChanged: changedImVal(checkState==Qt.Checked)
                    }
                    CheckBox {
                        id: vall
                        width: parent.width*(1-parent.ratio*2)
                        height: parent.height
                        text: parent.sigName+(parent.connected?"":"(Нет связи)")
                        indicator: Rectangle{
                            color: parent.parent.val?Fap.engTrueSignal:Fap.engFalseSignal
                            height: parent.parent.indicatorSize
                            width: parent.parent.indicatorSize
                            radius: width / 2
                            x: parent.leftPadding/2
                            y: (parent.height-height)/2
                            border.color: "#bdbdbd"
                        }
                    }
                }
            }
            Component{
                id: propertyValue
                Row{
                    property real val: 0
                    property bool connected: false
                    property alias text: name.text
                    property real ratio: 0.1
                    property int rowSize: 20
                    property int indicatorSize: 16
                    property string sigName: "Unknown signal"
                    width: parent==undefined?0:parent.width
                    height: rowSize
                    signal changedIm(bool status)
                    signal changedImVal(variant status)
                    function changeIm(status){
                        imit.checked = status?Qt.Checked:Qt.Unchecked
                    }
                    function changeImVal(status){
                        imitVal.text = status
                    }
                    function changeVal(status){
                        val = status
                    }
                    function changeConnected(status){
                        connected = status
                    }
                    spacing: 1
                    CheckBox {
                        id: imit
                        //                        width: parent.width*parent.ratio
                        width: parent.height
                        height: parent.height
                        indicator.width: parent.indicatorSize
                        indicator.height: parent.indicatorSize
                        onCheckStateChanged: changedIm(checkState==Qt.Checked)
                    }

                    TextField{
                        id: imitVal
                        width: parent.width*parent.ratio*2 - 1
                        height: 16
                        anchors.top: parent.top
                        anchors.topMargin: (rowSize - 16) / 2
                        text: "0"
                        onEditingFinished: changedImVal(parseInt(imitVal.text, 10))
                        validator: RegExpValidator {
                            regExp: /[0-9]+/
                        }
                        padding: 1
                        Keys.onPressed:{
                            if (event.key===16777220 || event.key===16777221){ //Enter
                                focus = false;
                            }
                        }
                    }
                    TextField{
                        id: vval
                        width: parent.width*parent.ratio*2
                        height: 16
                        anchors.top: parent.top
                        anchors.topMargin: (rowSize - 16) / 2
                        text: val
                        onEditingFinished: changedImVal(text)
                        readOnly: true
                        padding: 1
                        background: Rectangle{
                            anchors.fill: parent
                            radius: 4
                            border.width: 1
                            border.color: "#bdbdbd"
                            color: "white"
                        }
                    }
                    Text{
                        id: name
                        width: parent.width*parent.ratio*5
                        height: parent.height
                        text: " " + parent.sigName+(parent.connected?"":"(Нет связи)")
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
            Component{
                id: propertySetting
                Row{
                    id: root
                    property alias settingName: settingLable.text
                    property alias value: textField.text
                    property real ratio: 0.3
                    property int rowSize: 20
                    property bool timed: false
                    property bool reversed: false
                    width: parent==undefined?0:parent.width
                    height: rowSize
                    signal changedVal(variant val)
                    function changeVal(val){
                        //                        if(objectName == ".vsWarning_pulseDelay"){
                        //                           console.log("//------------")
                        //                        }
                        //                        console.log(objectName, val)
                        textField.text = val
                        textFieldNow.value = reversed?val:0
//                        if (!textFieldNow.timer.running){
//                            textFieldNow.stopCountdown()
//                        }
                    }
                    function commit(){
                        if(textField.text === ""){
                            textField.text = "0"
                        }
                        changedVal(parseFloat(textField.text, 10))
                    }
                    function startCountdown(sec){
                        textFieldNow.value = reversed?sec:0
                        timer.start()
                        //console.log("started", reversed)
                    }
                    function stopCountdown(){
                        textFieldNow.value = reversed?value:0
                        timer.stop()
                        //console.log("stopped")
                    }

                    TextField{
                        id: textFieldNow
                        property int value: 0
                        onValueChanged: {
                            let days =      ~~(value   / 86400            )
                            let hours =     ~~(value   / 3600      % 24   )
                            let minutes =   ~~(value   / 60        % 60   )
                            let seconds =   ~~(value               % 60   )

                            textFieldNow.text = (  days    > 0 ?  (days + "д ") : ""        ) +
                                                ((hours   + ":")                            ) +
                                                ((minutes > 9 ? "":"0") + (minutes + ":") ) +
                                                ((seconds > 9 ? "":"0") + seconds           )

                        }

                        height: 16
                        width: timed ? (parent.width*parent.ratio / 2) : 0
                        anchors.top: parent.top
                        anchors.topMargin: (rowSize - 16) / 2
                        clip: true
                        padding: 1
                        text: "0"
                        maximumLength: 15
                        enabled: timer.running
                        readOnly: true
                        Timer{
                            id: timer
                            interval: 1000
                            repeat: true
                            onTriggered: {
                                if ((reversed && textFieldNow.value === 0) || (!reversed && textFieldNow.value >= root.value)){
                                    stopCountdown()
                                }else{
                                    textFieldNow.value -= reversed?1:-1
                                }
                            }
                        }
//                        MouseArea{
//                            anchors.fill: parent
//                            onClicked: {
//                                startCountdown()
//                            }
//                        }
                    }
                    TextField{
                        id: textField
                        onTextChanged: {
                            textFieldNow.value = reversed ? text : "0";
                        }
                        height: 16
                        width: parent.width*parent.ratio / (timed ? 2 : 1)
                        anchors.top: parent.top
                        anchors.topMargin: (rowSize - 16) / 2
                        clip: true
                        padding: 1
                        text: "0"
                        maximumLength: 5
                        onEditingFinished: {
                            commit()
                        }
                        validator: RegExpValidator {
                            regExp: /([0-9]+)([.]?)([0-9]*)/
                        }
                        Keys.onPressed:{
                            if (event.key===16777220 || event.key===16777221){ //Enter
                                focus = false;
                            }
                        }
                    }
                    Text{
                        id: settingLable
                        height: parent.height
                        width: parent.width*(1-parent.ratio)
                        verticalAlignment: Text.AlignVCenter
                        text: "Unknown setting"
                        leftPadding: 6
                    }
                }

            }
            ObjectModel{
                id: engmodel
                property int index: 0
            }
            Repeater{
                model: engmodel
            }
            Rectangle{
                width: parent.width
                height: 1
                color: "gray"
                anchors.leftMargin: 5
                anchors.topMargin: 1
            }
        }
    }


    ObjectModel {
        id: alarms
        property int index: 0
    }

    ObjectModel {
        id: engSignals
        property int index: 0
    }

    Flickable  {
        id: sview
        anchors.fill: parent
        anchors.topMargin: 0
        ScrollBar.horizontal: ScrollBar { id: hbar; active: vbar.active }
        ScrollBar.vertical: ScrollBar { id: vbar; active: hbar.active }
        clip: true
        focus: true
        Keys.onUpPressed: ScrollBar.decrease()
        Keys.onDownPressed: ScrollBar.increase()
        contentWidth: parent.width
        contentHeight: windowContent.height
        Column{
            id: windowContent
            width: parent.width
            Row{
                id: areaHead
                width: parent.width
                height: buttonsType==4?0:20
                property real ratio: 0.8
                property alias buttonsType: areaButtons.buttonsType
                Item{
                    id: areaButtons
                    width: parent.width*parent.ratio
                    height: parent.height
                    property real ratio: 0.5
                    property int buttonsType: 0
                    Button {
                        property bool st: false
                        id: buttonStartStop
                        visible: parent.buttonsType==0
                        anchors.fill: parent
                        text: st?"Стоп":"Пуск"
                        onClicked: {if (st){
                                root.stop()
                            }else{
                                root.start()
                            }
                        }
                    }
                    Button {
                        property string baseText: parent.buttonsType>1?"Открыть":"Влево"
                        property bool st: false
                        property bool inSt: false
                        id: buttonLO
                        visible: parent.buttonsType>0
                        width: parent.width/2
                        height: parent.height
                        x: 0
                        y: 0
                        enabled: (buttonLO.st||buttonRC.st||!inSt)?true:false //inSt?false:true
                        text: buttonRC.st?"Стоп":baseText
                        onClicked:{ if (buttonLO.st||buttonRC.st){
                                root.stop()
                            }else{
                                root.start1()
                            }
                        }
                    }
                    Button {
                        property string baseText: parent.buttonsType>1?"Закрыть":"Вправо"
                        property bool st: false
                        property bool inSt: false
                        id: buttonRC
                        visible: parent.buttonsType>0
                        width: parent.width/2
                        height: parent.height
                        x: parent.width/2
                        y: 0
                        enabled: (buttonLO.st||buttonRC.st||!inSt)?true:false //inSt?false:true
                        text: buttonLO.st?"Стоп":baseText
                        onClicked: {if (buttonLO.st||buttonRC.st){
                                root.stop()
                            }else{
                                root.start2()
                            }
                        }
                    }
                }
                Rectangle{
                    id: areaCurrent
                    width: parent.width*(1-parent.ratio)
                    height: parent.height
                    color: Fap.currentBackground
                    Text {
                        id: currentBack
                        anchors.fill: parent
                        text: "888"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 17
                        font.family: "DSEG7 Classic"
                        color: Fap.currentBackgroundFont
                    }
                    Text {
                        id: currentTop
                        anchors.fill: parent
                        text: "---"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 17
                        font.family: "DSEG7 Classic"
                        color: Fap.currentFont
                    }
                }
            }
            Frame {
                id: areaAlarm
                width: parent.width
                height: alarmContent.height+padding*2
                padding: 4
                property bool alarm: false
                property bool alarmNotify: false
                property color alarmColor: Fap.alarm
                property color normalColor: Fap.contrBackground
                background: Rectangle{
                    color: parent.alarm?parent.alarmColor:parent.normalColor
                }
                Column{
                    id: alarmContent
                    width: parent.width
                    Row{
                        id: alarmHeader
                        width: parent.width
                        height: 14
                        property int fontSize: 10
                        property real ratio: 0.15
                        Text {
                            id: alarmTitleIgnor
                            width: parent.width*parent.ratio
                            height: parent.height
                            text: "Игнор"
                            horizontalAlignment: Text.AlignHLeft
                            verticalAlignment: Text.AlignTop
                            font.pixelSize: parent.fontSize
                        }
                        Text {
                            id: alarmTitleAlarm
                            width: parent.width*(1-parent.ratio)
                            height: parent.height
                            text: "Авария"
                            horizontalAlignment: Text.AlignHLeft
                            verticalAlignment: Text.AlignTop
                            font.pixelSize: parent.fontSize
                        }
                    }
                    Column{
                        id: alarmBody
                        width: parent.width
                        Repeater {
                            model: alarms
                        }
                    }
                    Item{
                        id: alarmButtons
                        width: parent.width
                        height: 20
                        Button {
                            id: buttonResetAlarm
                            anchors.fill: parent
                            text: "Сбросить ошибки"
                            onClicked: {
                                root.resetAlarm();
                                root.alarmNotify = false
//                                root.addAlarm("test1", "test", false)
//                                root.addEngRow("test2").addPropertySetting("test3", "test3", 0).changeVal(123)
                            }
                        }
                    }
                }
            }
            Column {
                id: areaHidden
                property int minimizeHiddenSize: 15
                property bool hidden: true
                width: parent.width
                Frame {
                    id: areaEngineer
                    visible: !parent.hidden
                    width: parent.width
                    height: visible?(engContent.height+padding*2):0
                    padding: 4
                    Column{
                        id: engContent
                        width: parent.width
                        Row{
                            id: engineerHeader
                            width: parent.width
                            height: 14
                            property int fontSize: 10
                            property real ratio: 0.1
                            Text {
                                id: engineerTitleEmit
                                width: parent.width*parent.ratio
                                height: parent.height
                                text: "Им."
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignTop
                                font.pixelSize: parent.fontSize
                            }
                            Text {
                                id: engineerTitleEmitValue
                                width: parent.width*parent.ratio
                                height: parent.height
                                text: "Им.зн"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignTop
                                font.pixelSize: parent.fontSize
                            }
                            Text {
                                id: engineerTitleValue
                                width: parent.width*(1-parent.ratio*2)
                                height: parent.height
                                text: "Значение сигнала"
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignTop
                                font.pixelSize: parent.fontSize
                            }
                        }
                        Column{
                            id: engineerSignals
                            width: parent.width
                            Repeater {
                                model: engSignals
                            }
                        }
                    }
                }
                //                Button {
                //                    id: buttonShowHidden
                //                    visible: true
                //                    width: parent.width
                //                    height: parent.minimizeHiddenSize
                //                    Text{
                //                        anchors.fill: parent
                //                        text: parent.parent.hidden?",":"."
                //                        font.family: "Guifx v2 Transports"
                //                        horizontalAlignment: Text.AlignHCenter
                //                        verticalAlignment: Text.AlignVCenter
                //                    }
                //                    onClicked: {
                //                        parent.hidden = !parent.hidden
                //                        console.log("test")
                //                    }
                //                }
            }
        }
    }
}
