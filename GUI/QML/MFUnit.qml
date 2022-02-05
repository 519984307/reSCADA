import QtQuick 2.15
//import QtQuick.Controls 1.3
import QtQuick.Controls 2.15
import "fap.js" as Fap

Item {
    id: mfu
    width: 120
    height: 30
    property color backgroundColor: "white"
    property color borderColor: "black"
    property int borderWidth: 1
    property alias textInput: valueLable
    property string tooltip: ""

    property bool checkLimit: false
    property bool correctingButtons: true
    property alias readOnly: valueLable.readOnly
    property real step: 0.5
    property real upLimit: 100
    property real downLimit: 0

    property int mantissa: 1
    property alias valueFontSize: valueLable.font
    property bool disappear: false
    //property alias valueText: valueLable.text
    property real valueReal: 999.9

    //property real extAlarmLevel: 0
    function setValue(value) {
        if (valueLable.text != value) {//!=выбран осознанно, т.к. text строка, а value число
            value = value.toFixed(mantissa)
            valueLable.limit = false
            valueLable.text = value
        }
    }
    function setValueLimited(value) {
        //уст с учетом лимитов
        value = Number(value)
        if (checkLimit) {
            if (value < mfu.downLimit) {
                value = mfu.downLimit
                setValue(value)
                valueStringChanged(value)
            }
            else if (value > mfu.upLimit) {
                value = mfu.upLimit
                setValue(value)
                valueStringChanged(value)
            }
            else setValue(value)
        }
        else {
            setValue(value)
        }
    }

    signal valueStringChanged(string value)
    onValueStringChanged: {
        //console.log( "New value - ", value )
        valueReal =  Number(value)
    }
    onValueRealChanged: {
        if( String(valueReal) !== valueLable.text ){
        //toFicsed округляет до нужной мантиссы, Number убирает ненужные нулитипа 12.32000
            valueLable.text = Number(valueReal.toFixed(mantissa))
           //console.log( objectName, valueReal )
        }
        //console.log( "New valueReal - ", valueReal )
    }

    Timer {
        id: timer
        interval: 1000
        running: false
        repeat: false
        property string buffer: ""
        onTriggered: {
            if (valueLable.text != buffer) {
                valueStringChanged(valueLable.text)
                //valueReal =  Number(valueLable.text)
            }
            buffer = ""
        }
    }
    MouseArea {
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
        Text {
            id: minBtn
            width: visible? height / 3 * 2 : 0
            visible: mfu.correctingButtons
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: valueLable.color
            text: "-"
            font.bold: true
            font.pixelSize: height * 0.7
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (timer.buffer == "")
                        timer.buffer = valueLable.text
                    setValueLimited(Number(valueLable.text) - step)
                    timer.start()
                }
            }
        }

        TextInput {
            id: valueLable
            property bool limit: true
            text: "999.9"
            anchors.left: minBtn.right
            anchors.right: maxBtn.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            font.pixelSize: height * 0.7
            font.family: "DSEG7 Classic"
            readOnly: false
            // Not Work
            //            validator: DoubleValidator {
            //                decimals: mantissa
            //                locale: "RU"
            //            }
            onEditingFinished: {
                if (checkLimit && Number(text) < downLimit) {
                    text = downLimit
                }
                valueStringChanged(text)
            }
            onTextChanged: {
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
                if (checkLimit && newVal > upLimit && limit) {
                    remove(text.length - 1, text.length)
                }
                limit = true //Чтобы не лимитировать значения от setValue
            }
        }

        Text {
            id: maxBtn
            width: visible? height / 3 * 2 : 0
            visible: mfu.correctingButtons
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: valueLable.color
            text: "+"
            font.bold: true
            font.pixelSize: height * 0.7
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (timer.buffer == "")
                        timer.buffer = valueLable.text
                    setValueLimited(Number(valueLable.text) + step)
                    timer.stop()
                    timer.start()
                }
            }
        }

        ToolTip {
            id: ttip
            delay: 2000
            timeout: 2000
            visible: false
            text: tooltip
        }



    }



}

/*##^##
Designer {
    D{i:0;height:35;width:102}
}
##^##*/

