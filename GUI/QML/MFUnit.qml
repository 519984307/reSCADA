import QtQuick 2.15
//import QtQuick.Controls 1.3
import QtQuick.Controls 2.15
import "fap.js" as Fap

Item {
    id: mfu
    width: 120
    height: 30
    property alias body: body
    property alias mainRect: body
    property alias mouseArea: mouseArea
    property alias minBtn: minBtn
    property alias maxBtn: maxBtn

    property color backgroundColor: "white"
    property color borderColor: "black"
    property int borderWidth: 1
    property alias textInput: valueLable
    property string tooltip: ""

    property bool limited: false
    property bool correctingButtons: true
    property alias readOnly: valueLable.readOnly
    property double step: 0.5
    property double upLimit: 100
    property double downLimit: 0

    property int mantissa: 1
    property alias valueFontSize: valueLable.font
    property bool disappear: false
    //property alias valueText: valueLable.text
    property double valueReal: 0
    property double valueRealTmp: 0

    property bool separCorrButtons: false
    property bool setFromCore: false

    signal s_more(bool More)
    signal s_less(bool Less)

    //property real extAlarmLevel: 0
    function setValue(value) {
        setFromCore = true
        valueReal = Number(value)
        //        if (valueLable.text != value) {//!=выбран осознанно, т.к. text строка, а value число
        //            value = value.toFixed(mantissa)
        //            value = Number(value)
        //            valueLable.limit = false
        //            valueLable.text = value
        //        }
    }
    function checkLimits(value) {
        //уст с учетом лимитов
        value = Number(value)
        value.toFixed(mantissa)
        value = Number(value)
        if( setFromCore || !limited )
            return value
        if (value < mfu.downLimit) {
            return mfu.downLimit
        }
        else if (value > mfu.upLimit) {
            return mfu.upLimit
        }
        else return value
    }
    signal valueChanged(variant Value)
    //onValueChanged: console.log( "valueChanged", valueReal )
    onValueRealChanged: {
        //console.log( "onValueRealChanged1", valueReal )
        if( String(valueReal) !== valueLable.text ){
            //toFicsed округляет до нужной мантиссы, Number убирает ненужные нулитипа 12.32000
            valueLable.text = checkLimits(valueReal)
            if(setFromCore) setFromCore = false
            //            if(setFromCore){
            //                console.log( "onValueRealChanged_setFromCore", valueReal )
            //                setFromCore = false
            //            }
            //            else if(valueLable.loaded){
            //            console.log( "onValueRealChanged_NosetFromCore", valueReal )
            //                valueChanged(valueReal)
            //            }
        }

    }

    Timer {
        id: timer
        interval: 1000
        running: false
        repeat: false
        property string buffer: ""
        onTriggered: {
            if (valueLable.text != buffer) {
                valueLable.editingFinished()
            }
            buffer = ""
        }
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        //onClicked: mfu.notified()
        hoverEnabled: true
        onEntered: {
            ttip.visible = true
            //TEST valueReal += 10
        }
        onExited: {
            ttip.visible = false
        }
        onContainsMouseChanged: {
            if (disappear) {
                if (containsMouse)
                    body.visible = true
                else
                    body.visible = false
            }
        }
    }

    Rectangle {
        id: body
        visible: !mfu.disappear
        anchors.fill: parent
        color: backgroundColor
        border.width: borderWidth
        border.color: borderColor


        TextInput {
            id: valueLable
            property bool limit: true
            property bool loaded: false
            property bool chdByUsr: false
            Component.onCompleted: {
                text = checkLimits(valueReal.toFixed(mantissa))
                loaded = true
            }
            anchors.left: minBtn.right
            anchors.right: maxBtn.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            font.pixelSize: mfu.height * 0.7
            //font.family: "DSEG7 Classic"
            readOnly: false
            // Not Work
            //            validator: DoubleValidator {
            //                decimals: mantissa
            //                locale: "RU"
            //            }
            onEditingFinished: {
                //console.log( "onEditingFinished", valueReal )
                text = checkLimits(text)
                if (text != valueReal) {//!=выбран осознанно, т.к. text строка, а value число
                    valueReal =  text
                    valueChanged(valueReal)
                }
            }
            onTextChanged: {
                //console.log( "onTextChanged", valueReal )
                if( text.charAt(text.length - 1) === ',' ){
                    remove(text.length - 1, text.length)
                    text += "."
                }
                if( text.charAt(text.length - 1) === '.' ){
                    if(text.indexOf('.',0) != (text.length - 1) )
                        remove(text.length - 1, text.length)
                }
                var newVal = text.indexOf('.')
                if( newVal != -1 && newVal < text.length - mantissa - 1 ){
                    remove(text.length - 1, text.length)
                }
                newVal = Number(text)

                if( isNaN(newVal) ){//validator: DoubleValidator { пропускает "," приходится её так отсекать
                    undo()
                    return
                }
                if (limited && (newVal > upLimit) && limit) {
                    remove(text.length - 1, text.length)
                }
                limit = true //Чтобы не лимитировать значения от setValue
            }
        }
        ToolTip {
            id: ttip
            delay: 2000
            timeout: 2000
            visible: false
            text: tooltip
        }
        SimpleButton {
            id: minBtn
            width: nameText.text.length * nameText.font.pixelSize * 0.7
            visible: mfu.correctingButtons
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 0
            anchors.bottomMargin: mfu.borderWidth
            anchors.topMargin: mfu.borderWidth
            unPressCheckColor: mfu.backgroundColor
            nameText.text: "-"
            nameText.font.pixelSize: valueLable.font.pixelSize
            border.width: 0
            anchors.left: parent.left
            radius: 0
            checkable: false
            mouseArea.onClicked: {
                if(!separCorrButtons){
                    if (timer.buffer == ""){
                        timer.buffer = valueLable.text
                    }
                    valueLable.text = checkLimits( Number(valueLable.text) - step )
                    timer.stop()
                    timer.start()
                }
            }
            onPressedChanged: s_less(pressed)
        }
        SimpleButton {
            id: maxBtn
            width: nameText.text.length * nameText.font.pixelSize * 0.7
            visible: mfu.correctingButtons
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            //anchors.rightMargin: mfu.borderWidth
            anchors.bottomMargin: mfu.borderWidth
            anchors.topMargin: mfu.borderWidth
            unPressCheckColor: mfu.backgroundColor
            nameText.text: "+"
            nameText.font.pixelSize: valueLable.font.pixelSize
            border.width: 0
            radius: 0
            checkable: false
            mouseArea.onClicked: {
                if(!separCorrButtons){
                    if (timer.buffer == ""){
                        timer.buffer = valueLable.text
                    }
                    valueLable.text = checkLimits( Number(valueLable.text) + step )
                    timer.stop()
                    timer.start()
                }
            }
            onPressedChanged: s_more( pressed )
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:2;height:35;width:102}
}
##^##*/

