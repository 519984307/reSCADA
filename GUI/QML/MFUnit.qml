import QtQuick 2.15
//import QtQuick.Controls 1.3
import QtQuick.Controls 2.15
import "fap.js" as Fap

Item {
    id: root
    width: 120
    height: 40
    property color  backgroundColor: "white"
    property color  borderColor: "black"
    property int    borderWidth: 1
    property alias  textInput: valueLable
    property string tooltip: ""

    property bool   checkLimit: true
    property bool   correctingButtons: true
    property alias  readOnly: valueLable.readOnly
    property real   step: 1
    property real   upLimit: 100
    property real   downLimit: 0

    property int    mantissa: 1
    property alias  valueFontSize: valueLable.font
    property bool   disappear: false
    property alias  valueText: valueLable.text
    property real   extAlarmLevel: 0
    property alias  editable: valueLable.readOnly

    function setValue(value){
        if (valueLable.text !== value){
            if (!Fap.isString(value)){
                value = value.toFixed(mantissa)
            }
            valueLable.limit = false
            valueLable.text = value
        }
    }

    function setValueLimited(value){//уст с учетом лимитов
        var newValue = Number(value)
        if( newValue < root.downLimit ){
            newValue = root.downLimit
            setValue( newValue)
            valueChanged( valueLable.text )
        }
        else if( newValue > root.upLimit ){
            newValue = root.upLimit
            setValue( newValue)
            valueChanged( valueLable.text )
        }
        else
            setValue( newValue)
    }

    signal valueChanged( string value )

    Timer {
        id: timer
        interval: 1000
        running: false
        repeat: false
        property string buffer: ""
        onTriggered: {
            if( valueLable.text != buffer ) {
                valueChanged( valueLable.text )
            }
            buffer = ""
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

        TextInput{
            id: valueLable
            property bool limit: true
            text: "99"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: height * 0.7
            font.family: "DSEG7 Classic"
            readOnly: false
            validator: DoubleValidator{
                decimals: mantissa
                locale: "GB"
            }
            onEditingFinished: {
                if (Number(text) < downLimit ) {
                    text = downLimit
                }
                valueChanged(text)
            }
            onTextChanged: {
                if ( Number(text) > upLimit && limit ) {
                    remove(text.length - 1, text.length)
                }
                limit = true
            }
            Text{
                width: height / 3*2
                visible: root.correctingButtons
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                text: "-"
                font.bold: true
                font.pixelSize: height * 0.7
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        if( timer.buffer == "" )
                            timer.buffer = valueLable.text
                        setValueLimited( Number(valueLable.text) - step )
                        timer.start()
                    }
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
                font.pixelSize: height * 0.7
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        if( timer.buffer == "" )
                            timer.buffer = valueLable.text
                        setValueLimited( Number(valueLable.text) + step )
                        timer.start()
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;height:35;width:102}
}
##^##*/
