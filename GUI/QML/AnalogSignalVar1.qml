import QtQuick 2.12
//import QtQuick.Controls 1.3
import QtQuick.Controls 2.12
import "fap.js" as Fap

Item {
    id: root
    width: 180
    height: 40
    property color backgroundColor: "white"
    property color colorShortName: "green"
    property color blinkColor: "yellow"
    property color borderColor: "black"
    property int borderWidth: 1
    //property int fontSize: 12
    property alias nameText: nameLable.text
    property alias valueText: valueLable.text
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
    property real extAlarmLevel: 0
    property bool allovAlarmBlinck: false
    property real realValue: 0

    SequentialAnimation{
        id: alarmFlah
        loops: Animation.Infinite
        running: false
        property color startColor: "green"
        ColorAnimation {
            target: rectValue
            property: "color"
            to: blinkColor
            duration: 1000
        }
        ColorAnimation {
            target: rectValue
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
            rectValue.color = alarmFlah.startColor
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
        rectValue.color = backgroundColor
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
            rectValue.color = rectValue.color == backgroundColor ? blinkColor : backgroundColor
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
                    rectValue.visible = true;
                else
                    rectValue.visible = false;
            }
        }
    }

    ToolTip{
        id: ttip
        delay: 2000
        timeout: 2000
        visible: false
        text: tooltip
    }
    Rectangle{
        id: rectShortName
        anchors.left: parent.left
        anchors.leftMargin: 0
        height: parent.height
        width: parent.height
        border.width: borderWidth
        color: colorShortName
        Text{
            id: nameLable
            text: qsTr("P")
            anchors.fill: parent
            font.pixelSize: parent.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
        }

    }

    Rectangle{
        id: rectValue
        color: colorsigValue
        border.width: borderWidth
        anchors.left: rectShortName.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        TextInput{
//            property bool editing: false
            text: "999.9"
            id: valueLable
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height * 0.8
            font.bold: true
            font.family: "DSEG7 Classic"
            font.italic: editing
            readOnly: true
//            Keys.onPressed:{
//                if (event.key===16777220 || event.key===16777221){ //Enter
//                    focus = false;
//                }
//            }
//            onFocusChanged: {
//                editing = focus
//                if(!focus && ! readOnly){
//                    if (checkLimit){
//                        changeLimited(0)
//                    }else{
//                        if (!Fap.isString(text)){
//                            text = text.toFixed(mantissa)
//                            realValue = Number( text )
//                        }
//                    }
//                    editing = false;
//                }
//            }
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:4;height:629;width:107}D{i:5;locked:true}
}
##^##*/
