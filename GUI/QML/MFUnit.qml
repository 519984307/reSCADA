import QtQuick 2.12
//import QtQuick.Controls 1.3
import QtQuick.Controls 2.12
import "fap.js" as Fap

Item {
    id: root
    width: 120
    height: 40
    property color backgroundColor: "white"
    property color blinkColor: "yellow"
    property color borderColor: "black"
    property int borderWidth: 1
    property int fontSize: 12
    property alias name: nameLable.text
    property alias text: valueLable.text
    property string tooltip: ""

    property bool checkLimit: true
    property bool correctingButtons: false
    property alias readOnly: valueLable.readOnly
    property real step: 1
    property int upLimit: 100
    property int downLimit: 0
    property alias regexp: valueLable.validator
    property int mantissa: 0
    property alias valueFontSize: valueLable.font
    property bool disappear: false
    property bool alarmBlinck: false
    property alias valueText: valueLable.text
    property real extAlarmLevel: 0
    property bool allovAlarmBlinck: false
    property real realValue: 0

    SequentialAnimation{
        id: alarmFlah
        loops: Animation.Infinite
        running: false
        property color startColor: "green"
        ColorAnimation {
            target: body
            property: "color"
            to: blinkColor
            duration: 1000
        }
        ColorAnimation {
            target: body
            property: "color"
            to: backgroundColor
            duration: 1000
        }
    }

    onAlarmBlinckChanged:{
        if( alarmBlinck ){
            //alarmFlah.startColor = backgroundColor
            alarmFlah.running = true
        }
        else{
            alarmFlah.running = false
            body.color = alarmFlah.startColor
        }
    }
    onExtAlarmLevelChanged: alarmBlinckCheck()
    onRealValueChanged: {
        valueChanged( realValue )
        alarmBlinckCheck()
    }
    onAllovAlarmBlinckChanged: alarmBlinckCheck()
    onBackgroundColorChanged: alarmFlah.startColor = backgroundColor

    function alarmBlinckCheck (){
        if( allovAlarmBlinck )
            root.alarmBlinck = realValue >= extAlarmLevel
        else
            alarmBlinck = false
    }
    function notified(){//прекр мигание
        timer.stop()
        body.color = backgroundColor
    }

    function notify(){//вкл мигание
        timer.start();
    }

    function setLable(lable){//уст значение с миганием
        setLableSilent(lable)
        notify();
    }

    function setLableSilent(lable){//уст значение без мигания
        if (root.text !== lable){
            if (!Fap.isString(lable)){
                lable = lable.toFixed(mantissa)
                realValue = Number( lable )
            }
            root.text = lable
        }
    }

    function changeLimited(value){//уст с учетом лимитов
        var newValue = Number(root.text) + value
        setLableSilent(newValue < root.downLimit ? root.downLimit : newValue > root.upLimit ? root.upLimit : newValue)
        valueChanged(root.text)
    }

    signal valueChanged(variant value)

    Timer{
        id: timer
        interval: 500
        onTriggered: {
            body.color = body.color == backgroundColor ? blinkColor : backgroundColor
        }
    }
    MouseArea{
        anchors.fill: parent
        onClicked: root.notified();
        hoverEnabled: true
        onEntered: {
            ttip.visible = true
        }
        onExited: {
            ttip.visible = false
        }
        onContainsMouseChanged: {
            if( disappear ){
                if( containsMouse )
                    body.visible = true;
                else
                    body.visible = false;
            }
        }
    }
    Rectangle{
        id: body
        visible: ! root.disappear
        anchors.fill: parent
        color: backgroundColor
        border.width: borderWidth
        border.color: borderColor
        ToolTip{
            id: ttip
            delay: 2000
            timeout: 2000
            visible: false
            text: tooltip
        }

        Text{
            id: nameLable
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: ""
            font.pixelSize: fontSize
        }

        TextInput{
            property bool editing: false
            text: "99"
            id: valueLable
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: nameLable.text == "" ? parent.top : nameLable.bottom
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.italic: editing
            readOnly: true
            Keys.onPressed:{
                if (event.key===16777220 || event.key===16777221){ //Enter
                    focus = false;
                }
            }
            onFocusChanged: {
                editing = focus
                if(!focus && ! readOnly){
                    if (checkLimit){
                        changeLimited(0)
                    }else{
                        if (!Fap.isString(text)){
                            text = text.toFixed(mantissa)
                            realValue = Number( text )
                        }
                    }
                    editing = false;
                }
            }

            Text{
                width: height / 3*2
                visible: root.correctingButtons
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                text: "-"
                font.bold: true
                font.pixelSize: 0
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
                MouseArea{
                    anchors.fill: parent
                    onClicked: root.changeLimited(-root.step);
                }
            }

            Text{
                width: height / 3*2
                visible: root.correctingButtons
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                text: "+"
                font.bold: true
                font.pixelSize: fontSize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
                MouseArea{
                    anchors.fill: parent
                    onClicked: root.changeLimited(root.step);
                }
            }

        }
    }
}

/*##^##
Designer {
    D{i:0;height:35;width:102}D{i:10}D{i:12}
}
##^##*/
